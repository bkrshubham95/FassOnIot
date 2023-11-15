//
// Created by ankush on 3/28/23.
//

#include "Request.h"
#include <boost/tokenizer.hpp>
using namespace boost;

Request::Request(const std::string &uid, const std::string &topic, Request::REQ_TYPE reqType) : uid(uid), topic(topic),
                                                                                                reqType(reqType) {}

Request::Request() {}

bool Request::parseRawRequest(const std::string &raw_request) {
    std::vector<std::string> tokenizedRequest;
    tokenizeRequest(raw_request, tokenizedRequest);
    if(tokenizedRequest.size() != 3)
        return false;

    if(!determineReqType(tokenizedRequest))
        return false;

    // TODO: Validate UID and Topic
    uid = std::string(tokenizedRequest[1]);
    topic = std::string(tokenizedRequest[2]);
    return true;

}

bool Request::determineReqType(const std::vector<std::string> &tokenizedRequest) {
    if(tokenizedRequest[0] == "PUB")
        reqType = PUB;
    else if(tokenizedRequest[0] == "SUB")
        reqType = SUB;
    else
        return false;
    return true;
}

void Request::tokenizeRequest(const std::string &raw_request, std::vector<std::string> &tokenizedString) const {
    char_separator<char> sep(":");
    tokenizer<char_separator<char>> tokens(raw_request, sep);
    for (const auto& t : tokens) {
        tokenizedString.push_back(t);
    }
}

const std::string &Request::getUid() const {
    return uid;
}

const std::string &Request::getTopic() const {
    return topic;
}

Request::REQ_TYPE Request::getReqType() const {
    return reqType;
}
