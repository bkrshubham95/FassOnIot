//
// Created by ankush on 3/28/23.
//

#include <sys/socket.h>
#include "Server.h"
#include <spdlog/spdlog.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(const std::string &hostname, int port, std::shared_ptr<std::queue<RequestPtr>> requestQueuePtr,
               std::shared_ptr<std::mutex> requestQueueLockPtr)
        : hostname(hostname), port(port), requestQueuePtr(requestQueuePtr), requestQueueLockPtr(requestQueueLockPtr) {
    createSocket();
    bindAndListen();
}

void Server::createSocket() {
    int iSetOption = 1;
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
               sizeof(iSetOption));
    if (socketFD < 0) {
        spdlog::error("Socket creation failed: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void Server::bindAndListen() {
    sockAddrIn.sin_family = AF_INET;
    // TODO: Bind to the specified address
    sockAddrIn.sin_addr.s_addr = INADDR_ANY;
    sockAddrIn.sin_port = htons(port);

    if (::bind(socketFD, (struct sockaddr *) &sockAddrIn, sizeof(sockAddrIn)) < 0) {
        spdlog::error("Failed to bind to port {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (::listen(socketFD, 100) < 0) {
        spdlog::error("Failed to listen ", strerror(errno));
        exit(EXIT_FAILURE);
    }

}

void Server::start() {
    size_t sockAddrLen = sizeof(sockAddrIn);
    char buffer[MAX_REQ_SIZE];
    // TODO: Clean this while loop into functions
    while(1) {
        int connectionFD = ::accept(socketFD, (struct sockaddr*) &sockAddrIn, (socklen_t *) &sockAddrLen);
        if(connectionFD < 0) {
            spdlog::error("Could not accept incoming connection: {}", strerror(errno));
        }
        ssize_t readCount = read(connectionFD, buffer, MAX_REQ_SIZE);
        if(readCount < 0) {
            spdlog::error("Could not read from accepted connection: {}", strerror(errno));
        }
        std::string bufferStr(buffer, readCount);
        std::string response;
        std::shared_ptr<Request> requestPtr(new Request());
        if(requestPtr->parseRawRequest(bufferStr)){
            std::lock_guard<std::mutex> lockGuard(*requestQueueLockPtr);
            response = "OK";
            requestQueuePtr->push(requestPtr);
        }
        else
            response = "ERROR: REQUEST PARSING FAILED";

        ssize_t writeCount = write(connectionFD, response.c_str(), response.size());
        if(writeCount < 0) {
            spdlog::error("Could not write to connection {}: {}", connectionFD, strerror(errno));
        }
        close(connectionFD);
    }
}
