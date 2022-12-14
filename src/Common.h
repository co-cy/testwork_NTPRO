#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>
#include <map>

static short port = 5555;

struct User {
  size_t userID;
  std::string login;
  std::string password;

  User() : userID(0) {};
  User(size_t userID, std::string &login, std::string &password) : userID(userID), login(login), password(password) {}

  explicit operator std::string() const {
    return "USER(login: '" + login + "', password: '" + password + "')";
  }
};

class UserBase {
  std::map<size_t, std::string> id_login;
  std::map<std::string, User> login_user;

 public:
  UserBase() = default;

  bool append(std::string &login, std::string &password) {
    if (login_user.find(login) != login_user.end())
      return false;
    size_t new_user_id = login_user.size();

    id_login[new_user_id] = login;
    login_user[login] = User(new_user_id, login, password);
    return true;
  }

  [[nodiscard]] User get_user(size_t user_id) const {
    auto iter = id_login.find(user_id);
    if (iter == id_login.end())
      return {};
    else
      return login_user.find(iter->second)->second;
  }
  [[nodiscard]] User get_user(std::string &login) const {
    auto iter = login_user.find(login);
    if (iter == login_user.end())
      return {};
    else
      return iter->second;
  }
  [[nodiscard]] bool find(std::string &login) const {
    return login_user.find(login) != login_user.end();
  }
  [[nodiscard]] bool find(size_t user_id) const {
    return id_login.find(user_id) != id_login.end();
  }
};

#endif //CLIENSERVERECN_COMMON_HPP
