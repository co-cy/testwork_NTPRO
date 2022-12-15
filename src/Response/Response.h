//
// Created by co_cy on 12.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_
#define TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_

#include <string>
#include <sstream>

#include "User.h"

namespace Response {

enum TypeRequest : char {
  TypeInvalid = 0,
  TypeBoolMessage = 1,
  TypeInfoUser,

};

struct BoolMessage {
  bool state;
  std::string message;

  explicit BoolMessage(bool state) : state(state) {};
  BoolMessage(bool state, std::string &comment) : state(state), message(comment) {};
  BoolMessage(bool state, std::string &&comment) : state(state), message(comment) {};
  explicit BoolMessage(std::istringstream &stream) : state() {
    stream >> state;
    getline(stream, message, '\0');
  };

  operator std::string() const {
    return std::string(1, TypeBoolMessage) + " " + std::to_string(state) + " " + message;
  }
};

struct InfoUser {
  size_t user_id;
  std::string login;
  double balance_rub;
  double balance_usd;

  explicit InfoUser(User &user)
      : user_id(user.user_id), login(user.login), balance_rub(user.balance_rub), balance_usd(user.balance_usd) {};
  explicit InfoUser(std::istringstream &stream) : user_id(0), balance_rub(0), balance_usd(0) {
    stream >> user_id >> login >> balance_rub >> balance_usd;
  };

  operator std::string() const {
    return std::string(1, TypeInfoUser) + " " + std::to_string(user_id) + " " + login + " "
        + std::to_string(balance_rub) + " " + std::to_string(balance_usd);
  }
};

}

#endif //TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_
