//
// Created by ankush on 4/17/23.
//

#include "PosixMessageQueueWrapper.h"
#include "messages/pub_sub_messages.pb.h"

PosixMessageQueueWrapper::PosixMessageQueueWrapper(const std::string &applicationId, const std::string &topic,
                                                   const std::string &pathname)
        : applicationId(applicationId), topic(topic), pathname(pathname) {
    spdlog::set_level(spdlog::level::debug);
}

void PosixMessageQueueWrapper::sendMessage(const std::string &payload) {
    Message message;
    message.set_topic(topic);
    message.set_payload(payload);
    std::string raw_message = message.SerializeAsString();
    key_t key = ftok(pathname.c_str(), std::hash<std::string>{}("PUB" + applicationId));
    if(key == -1) {
        spdlog::error("Could not convert path and identifier to IPC key for uid: {} : {}", applicationId, strerror(errno));
    }

    size_t messageQueueId = msgget(key, 0666 | IPC_CREAT);
    if(messageQueueId == -1) {
        spdlog::error("Message queue not initialized, message send failed");
        return;
    }

    MessageBuffer messageBuffer;
    messageBuffer.type = 1;
    strcpy(messageBuffer.payload, raw_message.c_str());
    int retVal = msgsnd(messageQueueId, &messageBuffer, sizeof(messageBuffer.payload), 0);

    if(retVal == -1) {
        spdlog::error("Could not send message: {}", strerror(errno));
    }
}

std::string PosixMessageQueueWrapper::recvMessage() {
    key_t key = ftok(pathname.c_str(), std::hash<std::string>{}("SUB" + applicationId));
    if(key == -1) {
        spdlog::error("Could not convert path and identifier to IPC key for uid: {} : {}", applicationId, strerror(errno));
    }

    size_t messageQueueId = msgget(key, 0666 | IPC_CREAT);
    if(messageQueueId == -1) {
        spdlog::error("Message queue not initialized, message send failed");
        return {""};
    }

    MessageBuffer messageBuffer;
    size_t retVal = msgrcv(messageQueueId, &messageBuffer, sizeof(messageBuffer.payload), 1, 0);
    if(retVal == -1) {
        spdlog::error("Could not receive message: {}", strerror(errno));
    }
    std::string raw_message(messageBuffer.payload, retVal - sizeof(messageBuffer.type));
    Message message;
    message.ParseFromString(raw_message);
    return message.payload();
}

bool PosixMessageQueueWrapper::registerPublisher(const std::string &hostname, const int &port) {
    return sendPubSubRequest("PUB", hostname, port);
}

bool PosixMessageQueueWrapper::registerSubscriber(const std::string &hostname, const int &port) {
    return sendPubSubRequest("SUB", hostname, port);
}

bool PosixMessageQueueWrapper::sendPubSubRequest(const std::string &requestPrefix, const std::string &hostname, const int &port) {
    int status, fd;
    struct sockaddr_in sockaddrIn;

    std::string buffer(1024, '\0');
    std::string request = requestPrefix + ":" + applicationId + ":" + topic;

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        spdlog::error("Socket creation failed: {}", strerror(errno));
    }

    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(port);

    if(inet_pton(AF_INET, "127.0.0.1", &sockaddrIn.sin_addr) <= 0) {
        spdlog::error("Server address not valid: {}", strerror(errno));
    }

    if((status = connect(fd, (struct sockaddr *)&sockaddrIn, sizeof(sockaddrIn))) < 0) {
        spdlog::error("Could not connect to server: {}", strerror(errno));
    }

    spdlog::info("Sending request to server");
    if(send(fd, request.c_str(), request.length(), 0) == -1) {
        spdlog::error("Could not send message to server: {}", strerror(errno));
    }

    if(read(fd, (void *) buffer.c_str(), buffer.length()) == -1) {
        spdlog::error("Could not read response from server: {}", strerror(errno));
    }

    close(fd);

    spdlog::debug("Response: {}", buffer);
    return buffer.find("OK") != std::string::npos;
}
