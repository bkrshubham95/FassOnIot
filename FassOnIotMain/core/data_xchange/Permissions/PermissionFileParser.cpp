//
// Created by ankush on 4/19/23.
//

#include "PermissionFileParser.h"
#include "spdlog/spdlog.h"
#include <sys/file.h>
#include <boost/tokenizer.hpp>
using namespace boost;

PermissionFileParser::PermissionFileParser(const std::string &pathname, std::shared_ptr<PermissionMap> permissionMapPtr) : pathname(pathname), permissionMapPtr(permissionMapPtr) {
    lockPathname = pathname + ".lock";
}

void PermissionFileParser::run() {
    int fd, lockFileFD;
    char data[MAX_DATA_SIZE];
    if(!lockFile(lockFileFD)) {
        spdlog::error("Could not lock file: {}", pathname);
    }
    if(!readData(fd, data)) {
        spdlog::error("Could not read data from file: {}", pathname);
    }
    if(!parseData(data)) {
        spdlog::error("Could not parse data from file: {}", pathname);
    }
    truncateAndUnlockFile(fd, lockFileFD);
}

bool PermissionFileParser::lockFile(int &fd) {
    if((fd = open(lockPathname.c_str(), O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1){
        spdlog::error("Could not open file: {}: {}", lockPathname, strerror(errno));
        return false;
    }

    if(flock(fd, LOCK_EX) != 0) {
        spdlog::error("Could not obtain lock on {}: {}", lockPathname, strerror(errno));
        return false;
    }

    return true;
}

bool PermissionFileParser::readData(int &fd, char *data) {
    if((fd = open(pathname.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1) {
        spdlog::error("Could not open file: {}: {}", pathname, strerror(errno));
        return false;
    }
    memset(data, 0, MAX_DATA_SIZE);

    if(read(fd, (void *) data, MAX_DATA_SIZE) == -1) {
        spdlog::error("Read failed: {}", strerror(errno));
        return false;
    }
    return true;
}

bool PermissionFileParser::parseData(char *buffer) {
    std::string data(buffer);
    if(data.empty())
        return true;
    std::vector<std::string> lines, appIdPubSubList;
    char_separator<char> newline("\n"), colon(":"), comma(",");

    tokenizer<char_separator<char>> tokens(data, newline);
    for(auto token: tokens) {
        remove(token.begin(), token.end(), '\r');
        remove(token.begin(), token.end(), '\n');
        remove(token.begin(), token.end(), ' ');
        lines.push_back(token);
    }

    for(auto const &line: lines) {
        tokens = tokenizer<char_separator<char>>(line, colon);
        for(const auto &token: tokens) {
            appIdPubSubList.push_back(token);
        }
        if(appIdPubSubList.size() != 3) {
            spdlog::warn("Ignoring malformed line: {}", line);
            continue;
        }
        std::string applicationId, pubList, subList;
        applicationId = appIdPubSubList[0];
        pubList = appIdPubSubList[1];
        subList = appIdPubSubList[2];
        tokens = tokenizer<char_separator<char>>(pubList, comma);
        for(const auto &pubTopic: tokens) {
            permissionMapPtr->addPublicationPermission(pubTopic, applicationId);
        }
        tokens = tokenizer<char_separator<char>>(subList, comma);
        for(const auto &subTopic: tokens) {
            permissionMapPtr->addSubscriptionPermission(subTopic, applicationId);
        }
        spdlog::debug("Processed line: {}", line);
        appIdPubSubList.clear();
    }
    return true;
}

bool PermissionFileParser::truncateAndUnlockFile(int &fd, int &lockFileFD) {
    if(ftruncate(fd, 0) == -1)  {
        spdlog::error("ftruncate failed: {}", strerror(errno));
    }
    if(close(fd) == -1) {
        spdlog::error("close failed: {}", strerror(errno));
    }
    if(flock(lockFileFD, LOCK_UN) != 0) {
        spdlog::error("Unlock failed: {}", strerror(errno));
    }
    return true;
}



