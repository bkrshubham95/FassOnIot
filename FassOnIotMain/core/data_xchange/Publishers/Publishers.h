//
// Created by ankush on 3/26/23.
//

#ifndef DATA_XCHANGE_PUBLISHERS_H
#define DATA_XCHANGE_PUBLISHERS_H


#include <unordered_map>
#include "UserApplication/UserApplication.h"
#include "Publisher.h"

class Publishers {
    PublisherSet publishers;

public:
    Publishers();
    void addPublisher(std::shared_ptr<Publisher> &publisherPtr);
    bool contains(PublisherPtr);
    PublisherPtr find(PublisherPtr publisherPtr);
    void checkAndProcessMessages();
};


#endif //DATA_XCHANGE_PUBLISHERS_H
