#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>
#include <map>

static short port = 5555;

struct User {
  std::string login;
  std::string password;

  User() = default;
  User(std::string login, std::string password) : login(login), password(password) {}

  operator std::string() const {
    return "USER(login: '" + login + "', password: '" + password + "')";
  }
};

class UserBase {
  std::map<std::string, std::string> database{};

 public:
  UserBase() = default;

  bool append(User &new_user) {
    if (database.find(new_user.login) == database.end()) {
      database[new_user.login] = new_user.password;
      return true;
    }
    return false;
  }

  [[nodiscard]] bool find(std::string login) const {
    return database.find(login) != database.end();
  }
  [[nodiscard]] bool find(User &user) const {
    auto iter = database.find(user.login);
    return iter != database.end() && iter->second == user.password;
  }
};

#endif //CLIENSERVERECN_COMMON_HPP
