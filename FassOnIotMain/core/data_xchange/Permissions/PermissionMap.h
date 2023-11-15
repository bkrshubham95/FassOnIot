//
// Created by ankush on 4/19/23.
//

#ifndef DATA_XCHANGE_PERMISSIONMAP_H
#define DATA_XCHANGE_PERMISSIONMAP_H

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>

using vsv = std::vector<std::unordered_set < std::string>>;

class PermissionMap {
    const int PUB_INDEX = 0, SUB_INDEX = 1;
    std::unordered_map<std::string, vsv> topicToPubSub;

    void addPermission(const int &vectorIndex, const std::string &topic, const std::string &applicationId);
    bool isAllowed(const int &vectorIndex, const std::string &topic, const std::string &applicationId);
public:
    PermissionMap();
    void addPublicationPermission(const std::string &topic, const std::string &applicationId);
    void addSubscriptionPermission(const std::string &topic, const std::string &applicationId);
    bool isPublicationAllowed(const std::string &topic, const std::string &applicationId);
    bool isSubscriptionAllowed(const std::string &topic, const std::string &applicationId);

};


#endif //DATA_XCHANGE_PERMISSIONMAP_H
