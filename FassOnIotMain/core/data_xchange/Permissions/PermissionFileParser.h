//
// Created by ankush on 4/19/23.
//

#ifndef DATA_XCHANGE_PERMISSIONFILEPARSER_H
#define DATA_XCHANGE_PERMISSIONFILEPARSER_H


#include <string>
#include <memory>
#include "PermissionMap.h"

class PermissionFileParser {
    const int MAX_DATA_SIZE = 1024;
    std::string pathname, lockPathname;
    std::shared_ptr<PermissionMap> permissionMapPtr;
    bool lockFile(int &fd);
    bool readData(int &fd, char *data);
    bool parseData(char *data);
    bool truncateAndUnlockFile(int &fd, int &lockFileFD);
public:
    void run();

    explicit PermissionFileParser(const std::string &pathname, std::shared_ptr<PermissionMap> permissionMapPtr);
};


#endif //DATA_XCHANGE_PERMISSIONFILEPARSER_H
