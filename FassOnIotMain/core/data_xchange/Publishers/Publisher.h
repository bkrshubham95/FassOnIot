//
// Created by ankush on 3/26/23.
//

#ifndef DATA_XCHANGE_PUBLISHER_H
#define DATA_XCHANGE_PUBLISHER_H


#include <memory>
#include "UserApplication/UserApplication.h"
#include "Messenger/Messenger.h"
#include "Topic/Topic.h"

class Publisher {
    UserApplicationPtr userApplicationPtr;
    std::vector<TopicPtr> topicPtrs;
    Messenger messenger;
public:
    explicit Publisher(UserApplicationPtr &userApplication);

    const UserApplicationPtr &getUserApplicationPtr() const;

    void addTopic(TopicPtr &topicPtr);
    void checkAndProcessMessage();
    void checkTopicMatchAndForward(const Message &message);
    friend bool operator==(std::shared_ptr<Publisher> publisherPtr1, std::shared_ptr<Publisher> publisherPtr2);

};

struct PublisherHash {
    size_t operator()(std::shared_ptr<Publisher> publisherPtr) const {
        return std::hash<std::string>()(publisherPtr->getUserApplicationPtr()->getId());
    }
};

bool operator==(std::shared_ptr<Publisher> publisherPtr1, std::shared_ptr<Publisher> publisherPtr2);

using PublisherPtr = std::shared_ptr<Publisher>;
using PublisherSet = std::unordered_set<PublisherPtr, PublisherHash>;

#endif //DATA_XCHANGE_PUBLISHER_H
