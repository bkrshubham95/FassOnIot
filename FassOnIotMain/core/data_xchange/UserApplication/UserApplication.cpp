//
// Created by ankush on 3/26/23.
//

#include "UserApplication.h"

UserApplication::UserApplication(const std::string &id) : id(id) {}

const std::string &UserApplication::getId() const {
    return id;
}

void UserApplication::setId(const std::string &id) {
    UserApplication::id = id;
}
