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
  TypeInfoUser,
  TypeSellerOrders,
  TypeBuyOrders,
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
    stream >> order.user_id >> is_buy >> order.count >> order.price;
  }

  operator std::string() const {
    return std::string(1, TypeCreateOrder) + " " + std::to_string(order.user_id) + " " + std::to_string(is_buy) + " "
        + std::to_string(order.count) + " "
        + std::to_string(order.price);
  }
};

struct InfoUser {
  size_t user_id;

  explicit InfoUser(size_t user_id) : user_id(user_id) {}
  explicit InfoUser(std::istringstream &stream) : user_id(0) {
    stream >> user_id;
  }

  operator std::string() const {
    return std::string(1, TypeInfoUser) + " " + std::to_string(user_id);
  }
};

struct InfoSellerOrders {
  size_t user_id;

  explicit InfoSellerOrders(size_t user_id) : user_id(user_id) {}
  explicit InfoSellerOrders(std::istringstream &stream) : user_id(0) {
    stream >> user_id;
  }

  operator std::string() const {
    return std::string(1, TypeSellerOrders) + " " + std::to_string(user_id);
  }
};

struct InfoBuyOrders {
  size_t user_id;

  explicit InfoBuyOrders(size_t user_id) : user_id(user_id) {}
  explicit InfoBuyOrders(std::istringstream &stream) : user_id(0) {
    stream >> user_id;
  }

  operator std::string() const {
    return std::string(1, TypeBuyOrders) + " " + std::to_string(user_id);
  }
};

}

#endif //TESTWORK_NTPRO_SRC_REQUEST_REQUEST_H_
