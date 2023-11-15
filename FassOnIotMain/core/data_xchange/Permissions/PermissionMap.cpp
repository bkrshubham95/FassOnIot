//
// Created by ankush on 4/19/23.
//

#include "PermissionMap.h"

PermissionMap::PermissionMap() {
}

void PermissionMap::addPublicationPermission(const std::string &topicToAdd, const std::string &applicationId) {
    addPermission(PUB_INDEX, topicToAdd, applicationId);
}

void PermissionMap::addSubscriptionPermission(const std::string &topicToAdd, const std::string &applicationId) {
    addPermission(SUB_INDEX, topicToAdd, applicationId);
}

bool PermissionMap::isPublicationAllowed(const std::string &topic, const std::string &applicationId) {
    return isAllowed(PUB_INDEX, topic, applicationId);
}

void PermissionMap::addPermission(const int &vectorIndex, const std::string &topic, const std::string &applicationId) {
    if(topicToPubSub.count(topic) == 0)
        topicToPubSub[topic] = vsv(2, std::unordered_set<std::string>());
    topicToPubSub[topic][vectorIndex].insert(applicationId);
}

bool PermissionMap::isAllowed(const int &vectorIndex, const std::string &topic, const std::string &applicationId) {
    if(topicToPubSub.count(topic) == 0)
        return false;
    return topicToPubSub[topic][vectorIndex].count(applicationId) > 0;
}

bool PermissionMap::isSubscriptionAllowed(const std::string &topic, const std::string &applicationId) {
    return isAllowed(SUB_INDEX, topic, applicationId);
}

