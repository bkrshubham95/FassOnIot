//
// Created by ankush on 3/28/23.
//

#ifndef DATA_XCHANGE_REQUEST_H
#define DATA_XCHANGE_REQUEST_H


#include <string>
#include <vector>
#include <memory>

class Request {
public:
    enum REQ_TYPE {PUB, SUB};

    Request();

    Request(const std::string &uid, const std::string &topic, REQ_TYPE reqType);

    bool parseRawRequest(const std::string &raw_request);

    const std::string &getUid() const;

    const std::string &getTopic() const;

    REQ_TYPE getReqType() const;

private:
    std::string uid, topic;
    REQ_TYPE reqType;

    void tokenizeRequest(const std::string &raw_request, std::vector<std::string> &tokenizedString) const;

    bool determineReqType(const std::vector<std::string> &tokenizedRequest);
};

using RequestPtr = std::shared_ptr<Request>;


#endif //DATA_XCHANGE_REQUEST_H
