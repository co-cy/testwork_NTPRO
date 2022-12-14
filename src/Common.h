#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>
#include <map>

static short port = 5555;

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

#endif //CLIENSERVERECN_COMMON_HPP
