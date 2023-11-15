//
// Created by ankush on 4/17/23.
//

#ifndef DATA_XCHANGE_POSIXMESSAGEQUEUEWRAPPER_H
#define DATA_XCHANGE_POSIXMESSAGEQUEUEWRAPPER_H

#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <string>
#include <sys/msg.h>
#include "spdlog/spdlog.h"

class PosixMessageQueueWrapper {
private:
    struct MessageBuffer {
        long type;
        char payload[1024];
    };
    const int MessageQueueType = 1;
    std::string applicationId, topic, pathname;
    bool sendPubSubRequest(const std::string &requestPrefix, const std::string &hostname, const int &port);
public:
    void sendMessage(const std::string &payload);
    std::string recvMessage();
    bool registerPublisher(const std::string &hostname, const int &port);
    bool registerSubscriber(const std::string &hostname, const int &port);

    PosixMessageQueueWrapper(const std::string &applicationId, const std::string &topic, const std::string &pathname);


};


#endif //DATA_XCHANGE_POSIXMESSAGEQUEUEWRAPPER_H
