//
// Created by ankush on 3/26/23.
//

#include "Publishers.h"

Publishers::Publishers() {}

void Publishers::addPublisher(std::shared_ptr<Publisher> &publisherPtr) {
    publishers.insert(publisherPtr);
}

bool Publishers::contains(PublisherPtr publisherPtr) {
    return publishers.find(publisherPtr) != publishers.end();
}

PublisherPtr Publishers::find(PublisherPtr publisherPtr) {
    return *publishers.find(publisherPtr);
}

void Publishers::checkAndProcessMessages() {
    for(PublisherPtr publisherPtr: publishers) {
        publisherPtr->checkAndProcessMessage();
    }

}

