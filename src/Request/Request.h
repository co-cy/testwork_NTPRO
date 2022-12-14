//
// Created by co_cy on 12.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_REQUEST_REQUEST_H_
#define TESTWORK_NTPRO_SRC_REQUEST_REQUEST_H_

#include <string>
#include <sstream>
#include "Common.h"
#include "Order.h"

namespace Request {

enum TypeRequest : char {
  TypeRegistration = 1,
  TypeAuth,
  TypeCreateOrder,
};

struct Registration {
  std::string login;
  std::string password;

  Registration(std::string &login, std::string &password) : login(login), password(password) {}

  explicit Registration(std::istringstream &stream) {
    stream >> login >> password;
  }

  operator std::string() const {
    return std::string(1, TypeRegistration) + " " + login + " " + password;
  }
};

struct Auth {
  std::string login;
  std::string password;

  Auth(std::string &login, std::string &password) : login(login), password(password) {}

  explicit Auth(std::istringstream &stream) {
    stream >> login >> password;
  }

  operator std::string() const {
    return std::string(1, TypeAuth) + " " + login + " " + password;
  }
};

struct CreateOrder {
  bool is_buy;
  Order order;

  CreateOrder(size_t userID, std::size_t count, double price) : is_buy(true), order(userID, count, price) {}
  CreateOrder(bool is_buy, size_t userID, std::size_t count, double price)
      : is_buy(is_buy), order(userID, count, price) {}
  explicit CreateOrder(std::istringstream &stream) : is_buy(true) {
    stream >> is_buy >> order.user_id >> order.count >> order.price;
  }

  operator std::string() const {
    return std::string(1, TypeCreateOrder) + " " + std::to_string(is_buy) + " " + std::to_string(order.user_id) + " "
        + std::to_string(order.count) + " "
        + std::to_string(order.count);
  }
};

}

#endif //TESTWORK_NTPRO_SRC_REQUEST_REQUEST_H_
