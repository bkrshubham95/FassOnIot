//
// Created by ankush on 3/26/23.
//

#ifndef DATA_XCHANGE_USERAPPLICATION_H
#define DATA_XCHANGE_USERAPPLICATION_H


#include <string>
#include <memory>

class UserApplication {
    std::string id;
public:
    const std::string &getId() const;

    void setId(const std::string &id);

public:
    explicit UserApplication(const std::string &id);
};

using UserApplicationPtr = std::shared_ptr<UserApplication>;

#endif //DATA_XCHANGE_USERAPPLICATION_H
