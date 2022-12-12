//
// Created by co_cy on 12.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_REQUEST_REQUEST_H_
#define TESTWORK_NTPRO_SRC_REQUEST_REQUEST_H_

#include <string>
#include <sstream>
#include "Common.h"

namespace Request {

enum TypeRequest : char {
  TypeRegistration = 1,
  TypeAuth,

};

struct Registration {
  User user;

  Registration(std::string login, std::string password) : user(login, password) {}
  explicit Registration(User &user) : user(user) {}

  explicit Registration(std::istringstream &stream) {
    stream >> user.login >> user.password;
  }

  operator std::string() const {
    return std::string(1, TypeRegistration) + " " + user.login + " " + user.password;
  }
};

struct Auth {
  User user;

  Auth(std::string login, std::string password) : user(login, password) {}
  explicit Auth(User &user) : user(user) {}

  explicit Auth(std::istringstream &stream) {
    stream >> user.login >> user.password;
  }

  operator std::string() const {
    return std::string(1, TypeAuth) + " " + user.login + " " + user.password;
  }
};

}

#endif //TESTWORK_NTPRO_SRC_REQUEST_REQUEST_H_
