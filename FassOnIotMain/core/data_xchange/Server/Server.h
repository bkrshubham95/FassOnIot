//
// Created by ankush on 3/28/23.
//

#ifndef DATA_XCHANGE_SERVER_H
#define DATA_XCHANGE_SERVER_H


#include <string>
#include <memory>
#include <mutex>
#include <queue>
#include <netinet/in.h>
#include "Server/Request/Request.h"

class Server {
    const int MAX_REQ_SIZE = 1024;
    std::string hostname;
    int port;
    int socketFD;
    sockaddr_in sockAddrIn;
    std::shared_ptr<std::queue<RequestPtr>> requestQueuePtr;
    std::shared_ptr<std::mutex> requestQueueLockPtr;

    void createSocket();
    void bindAndListen();
public:
    Server(const std::string &hostname, int port, std::shared_ptr<std::queue<RequestPtr>> requestQueuePtr,
           std::shared_ptr<std::mutex> requestQueueLockPtr);
    void start();

};


#endif //DATA_XCHANGE_SERVER_H
