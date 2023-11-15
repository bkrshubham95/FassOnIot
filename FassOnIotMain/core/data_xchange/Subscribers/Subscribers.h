//
// Created by ankush on 3/26/23.
//

#ifndef DATA_XCHANGE_SUBSCRIBERS_H
#define DATA_XCHANGE_SUBSCRIBERS_H


#include <vector>
#include <memory>
#include "UserApplication/UserApplication.h"
#include "Subscriber.h"

class Subscribers {
    SubscriberSet subscribers;

public:
    void addSubscriber(std::shared_ptr<Subscriber> &subscriberPtr);
    void sendMessage(const Message &message);

    bool contains(SubscriberPtr subscriberPtr);

    SubscriberPtr find(SubscriberPtr subscriberPtr);
};


#endif //DATA_XCHANGE_SUBSCRIBERS_H
