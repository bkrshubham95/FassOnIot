//
// Created by ankush on 3/27/23.
//

#include "Topic.h"

#include <utility>

Topic::Topic(std::string name) : name(std::move(name)) {
    subscriberPtrs = std::make_shared<Subscribers>();
}

void Topic::addSubscriber(SubscriberPtr &subscriberPtr) {
    subscriberPtrs->addSubscriber(subscriberPtr);
}

bool Topic::isMatch(const std::string &otherName) {
    return name == otherName;
}

void Topic::forwardMessageToSubscribers(const Message &message) {
    subscriberPtrs->sendMessage(message);
}

const std::string &Topic::getName() const {
    return name;
}

bool operator==(std::shared_ptr<Topic> topicPtr1, std::shared_ptr<Topic> topicPtr2) {
return topicPtr1->getName() == topicPtr2->getName();
}
