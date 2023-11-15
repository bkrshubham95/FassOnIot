//
// Created by ankpath on 3/26/23.
//

#include <queue>
#include <mutex>
#include <spdlog/spdlog.h>
#include "Server/Request/Request.h"
#include "Topic/Topic.h"
#include "Publishers/Publishers.h"
#include "Server/Server.h"
#include "Permissions/PermissionMap.h"
#include "Permissions/PermissionFileParser.h"

void startDataXchangeServer(std::shared_ptr<std::queue<RequestPtr>> requestQueuePtr, std::shared_ptr<std::mutex> requestQueueLockPtr) {
    Server server("localhost", 8000, requestQueuePtr, requestQueueLockPtr);
    server.start();
}

void sig_handler(int signal_num) {
    std::cout << "Bye bye." << std::endl;
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    spdlog::set_level(spdlog::level::debug);
    std::shared_ptr<std::queue<RequestPtr>> requestQueuePtr = std::make_shared<std::queue<RequestPtr>>();
    std::shared_ptr<std::mutex> requestQueueLockPtr = std::make_shared<std::mutex>();
    std::shared_ptr<PermissionMap> permissionMapPtr = std::make_shared<PermissionMap>();
    std::shared_ptr<PermissionFileParser> permissionFileParserPtr = std::make_shared<PermissionFileParser>("pub_sub_details", permissionMapPtr);
    TopicSet topicSet;
    Publishers publishers;
    Subscribers subscribers;


    std::thread serverThread(startDataXchangeServer, requestQueuePtr, requestQueueLockPtr);
    while (1) {
        permissionFileParserPtr->run();
        sleep(2);
        RequestPtr requestPtr;
        {
            std::lock_guard<std::mutex> lockGuard(*requestQueueLockPtr);
            if(!requestQueuePtr->empty()) {
                requestPtr = requestQueuePtr->front();
                requestQueuePtr->pop();
            }
        }
        if(!requestPtr)
            continue;
        TopicPtr topicPtr = std::make_shared<Topic>(requestPtr->getTopic());
        if(topicSet.count(topicPtr) == 0)
            topicSet.insert(topicPtr);
        else
            topicPtr = *topicSet.find(topicPtr);
        UserApplicationPtr userApplicationPtr = std::make_shared<UserApplication>(requestPtr->getUid());

        switch(requestPtr->getReqType()) {
            case Request::PUB: {
                if(!permissionMapPtr->isPublicationAllowed(topicPtr->getName(), userApplicationPtr->getId())){
                    spdlog::warn("Application: {} not allowed to publish to {}", userApplicationPtr->getId(), topicPtr->getName());
                    continue;
                }
                PublisherPtr publisherPtr = std::make_shared<Publisher>(userApplicationPtr);
                if (publishers.contains(publisherPtr))
                    publisherPtr = publishers.find(publisherPtr);
                else
                    publishers.addPublisher(publisherPtr);
                publisherPtr->addTopic(topicPtr);
            }
                break;
            case Request::SUB: {
                if(!permissionMapPtr->isSubscriptionAllowed(topicPtr->getName(), userApplicationPtr->getId())) {
                    spdlog::warn("Application: {} not allowed to subscribe to {}", userApplicationPtr->getId(),
                                 topicPtr->getName());
                    continue;
                }
                SubscriberPtr subscriberPtr = std::make_shared<Subscriber>(userApplicationPtr);
                if(subscribers.contains(subscriberPtr))
                    subscriberPtr = subscribers.find(subscriberPtr);
                else
                    subscribers.addSubscriber(subscriberPtr);
                topicPtr->addSubscriber(subscriberPtr);
            }
                break;
            default:
                spdlog::error("Request type not supported");
        }
        publishers.checkAndProcessMessages();
    }
    return 0;
}
