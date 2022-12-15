//
// Created by co_cy on 12.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_
#define TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_

#include <sstream>
#include <string>
#include <vector>
#include <set>

#include "Order.h"
#include "User.h"

namespace Response {

enum TypeRequest : char {
  TypeInvalid = 0,
  TypeBoolMessage = 1,
  TypeInfoUser,
  TypeInfoSellerOrders,
  TypeInfoBuyOrders,

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

struct InfoSellerOrders {
  size_t count_orders;
  std::vector<Order> orders;

  InfoSellerOrders() : count_orders(0) {}
  explicit InfoSellerOrders(std::istringstream &stream) : count_orders(0) {
    stream >> count_orders;
    for (int i = 0; i < count_orders; ++i)
      orders.emplace_back(stream);
  };

  void append(const Order &order) {
    orders.push_back(order);
    ++count_orders;
  }

  operator std::string() const {
    std::string tmp;
    for (const auto &order : orders) {
      tmp += " " + std::string(order);
    }
    return std::string(1, TypeInfoSellerOrders) + " " + std::to_string(count_orders) + " " + tmp;
  }
};

struct InfoBuyOrders {
  size_t count_orders;
  std::vector<Order> orders;

  InfoBuyOrders() : count_orders(0) {}
  explicit InfoBuyOrders(std::istringstream &stream) : count_orders(0) {
    stream >> count_orders;
    for (int i = 0; i < count_orders; ++i)
      orders.emplace_back(stream);
  };

  void append(const Order &order) {
    orders.push_back(order);
    ++count_orders;
  }

  operator std::string() const {
    std::string tmp;
    for (const auto &order : orders) {
      tmp += " " + std::string(order);
    }
    return std::string(1, TypeInfoBuyOrders) + " " + std::to_string(count_orders) + " " + tmp;
  }
};

}

#endif //TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_
