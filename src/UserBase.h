//
// Created by co_cy on 14.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_USERBASE_H_
#define TESTWORK_NTPRO_SRC_USERBASE_H_

#include <string>
#include <map>

#include "User.h"

class UserBase {
  std::map<size_t, std::string> id_login;
  std::map<std::string, User> login_user;

 public:
  UserBase() = default;

  size_t append(std::string &login, std::string &password) {
    if (login_user.find(login) != login_user.end())
      return 0;
    size_t new_user_id = login_user.size() + 1;

    id_login[new_user_id] = login;
    login_user[login] = User(new_user_id, login, password);
    return new_user_id;
  }

  [[nodiscard]] User &get_user(size_t user_id) {
    return login_user[id_login[user_id]];
  }
  [[nodiscard]] User &get_user(std::string &login) {
    return login_user[login];
  }
  [[nodiscard]] bool find(std::string &login) const {
    return login_user.find(login) != login_user.end();
  }
  [[nodiscard]] bool find(size_t user_id) const {
    return id_login.find(user_id) != id_login.end();
  }
};

static UserBase database{};

#endif //TESTWORK_NTPRO_SRC_USERBASE_H_
