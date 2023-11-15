//
// Created by ankush on 4/15/23.
//

#include <string>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "pub_sub_example.h"
#include "spdlog/spdlog.h"
#include "messages/pub_sub_messages.pb.h"

struct MessageBuffer {
    long type;
    char payload[1024];
};

void send_request(const std::string &request, std::string &buffer) {
    int status, fd;
    struct sockaddr_in sockaddrIn;

    buffer.clear();
    buffer.resize(1024);
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        spdlog::error("Socket creation failed: {}", strerror(errno));
    }

    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(8000);

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
    spdlog::info("Length: {}", buffer.length());

    if(read(fd, (void *) buffer.c_str(), buffer.length()) == -1) {
        spdlog::error("Could not read response from server: {}", strerror(errno));
    }

    close(fd);
}

void send_message(Message &message) {
    std::string uid = "123456";
    std::string pathname = "faas_on_iot";
    std::string raw_message = message.SerializeAsString();
    key_t key = ftok(pathname.c_str(), std::hash<std::string>{}(uid));
    if(key == -1) {
        spdlog::error("Could not convert path and identifier to IPC key for uid: {} : {}", uid, strerror(errno));
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

void recvMessage(Message &message) {
    std::string uid = "1234567";
    std::string pathname = "faas_on_iot";
    key_t key = ftok(pathname.c_str(), std::hash<std::string>{}(uid));
    if(key == -1) {
        spdlog::error("Could not convert path and identifier to IPC key for uid: {} : {}", uid, strerror(errno));
    }

    size_t messageQueueId = msgget(key, 0666 | IPC_CREAT);
    if(messageQueueId == -1) {
        spdlog::error("Message queue not initialized, message send failed");
        return;
    }

    MessageBuffer messageBuffer;
    size_t retVal = msgrcv(messageQueueId, &messageBuffer, sizeof(messageBuffer.payload), 1, 0);
    if(retVal == -1) {
        spdlog::error("Could not receive message: {}", strerror(errno));
    }
    std::string raw_message(messageBuffer.payload, retVal - sizeof(messageBuffer.type));
    message.ParseFromString(raw_message);
}

int main() {
    // 1. Register publisher and subscriber
    std::string pub_request, sub_request, buffer(1024, '\0');
    int fd, status;
    pub_request = "PUB:123456:sample_topic";
    sub_request = "SUB:1234567:sample_topic";

    spdlog::info("Sending {} to server", pub_request);
    send_request(pub_request, buffer);
    spdlog::info("Response from server: {}", buffer);

    spdlog::info("Sending {} to server: ", sub_request);
    send_request(sub_request, buffer);
    spdlog::info("Response from server: {}", buffer);

    sleep(5);
    // 2. Publish message
    std::string pub_message = "Hello from Publisher";
    Message message, recvedMessage;
    message.set_topic("sample_topic");
    message.set_payload(pub_message);
    spdlog::info("Sending message to server: Topic: {} Payload: {}", message.topic(), message.payload());
    send_message(message);
    sleep(5);
    // 3. Retrieve message
    spdlog::info("Receiving message from queue");
    recvMessage(recvedMessage);
    spdlog::info("Received message: Topic: {} Payload: {}", recvedMessage.topic(), recvedMessage.payload());

}