//
// Created by co_cy on 14.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_USER_H_
#define TESTWORK_NTPRO_SRC_USER_H_

#include <string>

struct User {
  size_t user_id;
  std::string login;
  std::string password;

  double balance_rub;
  double balance_usd;

  User() : user_id(0), balance_rub(0), balance_usd(0) {};
  User(size_t userID, std::string &login, std::string &password)
      : user_id(userID), login(login), password(password), balance_rub(0), balance_usd(0) {}

  explicit operator std::string() const {
    return "USER(login: '" + login + "', password: '" + password + "')";
  }
};

#endif //TESTWORK_NTPRO_SRC_USER_H_
