//
// Created by ankush on 3/26/23.
//

#ifndef DATA_XCHANGE_SUBSCRIBER_H
#define DATA_XCHANGE_SUBSCRIBER_H


#include <memory>
#include "UserApplication/UserApplication.h"
#include "Messenger/Messenger.h"

class Subscriber {
    UserApplicationPtr userApplicationPtr;
    Messenger messenger;
public:
    Subscriber(std::shared_ptr<UserApplication> userApplicationPtr);

    const UserApplicationPtr &getUserApplicationPtr() const;

    void sendMessage(const Message &message);

    friend bool operator==(std::shared_ptr<Subscriber> subscriberPtr1, std::shared_ptr<Subscriber> subscriberPtr2);

};

struct SubscriberHash {
    size_t operator()(std::shared_ptr<Subscriber> subscriberPtr) const {
        return std::hash<std::string>()(subscriberPtr->getUserApplicationPtr()->getId());
    }
};

bool operator==(std::shared_ptr<Subscriber> subscriberPtr1, std::shared_ptr<Subscriber> subscriberPtr2);

using SubscriberPtr = std::shared_ptr<Subscriber>;
using SubscriberSet = std::unordered_set<SubscriberPtr, SubscriberHash>;

#endif //DATA_XCHANGE_SUBSCRIBER_H
