//
// Created by ankush on 3/26/23.
//

#include "Messenger.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <spdlog/spdlog.h>


const std::string Messenger::pathname = "faas_on_iot";
const int Messenger::DEFAULT_MSG_TYPE = 1;
const int Messenger::MAX_MSG_LEN = 1024;

Messenger::Messenger(std::string uid) {
   spdlog::info("Creating messenger for {}", uid);
   key_t key = ftok(pathname.c_str(), std::hash<std::string>{}(uid));
   if(key == -1) {
       spdlog::error("Could not convert path and identifier to IPC key for uid: {} : {}", uid, strerror(errno));
   }

   messageQueueId = msgget(key, 0666 | IPC_CREAT);

   if(messageQueueId == -1) {
       spdlog::error("Could not create/get message queue for UID: {} : {}", uid, strerror(errno));
   }
}

void Messenger::send(const std::string& raw_message) {
    spdlog::info("Sending message queue: {}", messageQueueId);
    if(messageQueueId == -1) {
        spdlog::error("Message queue not initialized, message send failed");
        return;
    }

    if(raw_message.length() > MAX_MSG_LEN){
        spdlog::error("Message length greater than {} no allowed, send failed", MAX_MSG_LEN);
        return;
    }

    MessageBuffer messageBuffer;
    messageBuffer.type = DEFAULT_MSG_TYPE;
    strcpy(messageBuffer.payload, raw_message.c_str());
    int retVal = msgsnd(messageQueueId, &messageBuffer, sizeof(messageBuffer.payload), 0);

    if(retVal == -1) {
        spdlog::error("Could not send message: {}", strerror(errno));
    }
}

void Messenger::recv(std::string &raw_message) const {
    if(messageQueueId == -1) {
        spdlog::error("Message queue not initialized, message send failed");
        return;
    }

    MessageBuffer messageBuffer;
    size_t retVal = msgrcv(messageQueueId, (struct MessageBuffer *)&messageBuffer, sizeof(messageBuffer.payload), DEFAULT_MSG_TYPE, 0);
    if(retVal == -1) {
        spdlog::error("Could not receive message: {}", strerror(errno));
        return;
    }
    //return std::string(messageBuffer.payload, retVal - sizeof(messageBuffer.type));
    raw_message.resize(retVal - sizeof(messageBuffer.type));
    raw_message.assign(messageBuffer.payload);
}

bool Messenger::isMessagePending() {
    int retVal;
    struct msqid_ds buff;
    retVal = msgctl(messageQueueId, IPC_STAT, &buff);
    if(retVal == -1) {
        spdlog::error("msgctl on {} failed: {}", messageQueueId, strerror(errno));
        return false;
    }
    return buff.msg_qnum > 0;
}

Message Messenger::recvAndDeserializeMessage() {
    std::string raw_message;
    recv(raw_message);
    Message message;
    message.ParseFromString(raw_message);
    return message;
}

void Messenger::serializeAndSendMessage(const Message &message) {
    spdlog::info("Serializing and sending message: Topic: {} Payload: {}", message.topic(), message.payload());
    send(message.SerializeAsString());
    spdlog::info("Message should be sent");
}

