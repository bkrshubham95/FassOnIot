//
// Created by ankush on 3/27/23.
//

#ifndef DATA_XCHANGE_TOPIC_H
#define DATA_XCHANGE_TOPIC_H


#include <string>
#include <vector>
#include <memory>
#include "Subscribers/Subscribers.h"
#include "Subscribers/Subscriber.h"

class Topic {
    std::string name;
    std::shared_ptr<Subscribers> subscriberPtrs;
public:
    explicit Topic(std::string name);

    const std::string &getName() const;

    void addSubscriber(SubscriberPtr &subscriberPtr);
    bool isMatch(const std::string &otherName);
    void forwardMessageToSubscribers(const Message &message);

    friend bool operator==(std::shared_ptr<Topic> topicPtr1, std::shared_ptr<Topic> topicPtr2);

};

struct TopicHash {
    size_t operator()(std::shared_ptr<Topic> topicPtr) const {
        return std::hash<std::string>()(topicPtr->getName());
    }
};

bool operator==(std::shared_ptr<Topic> topicPtr1, std::shared_ptr<Topic> topicPtr2);

using TopicPtr = std::shared_ptr<Topic>;
using TopicSet = std::unordered_set<TopicPtr, TopicHash>;

#endif //DATA_XCHANGE_TOPIC_H
