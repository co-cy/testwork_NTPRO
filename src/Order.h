//
// Created by co_cy on 11.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_ORDER_H_
#define TESTWORK_NTPRO_SRC_ORDER_H_

#include <stdexcept>
#include <queue>
#include <vector>

#include "Common.h"

struct Order {
  size_t user_id;

  size_t count;
  double price;

  Order() : user_id(0), count(0), price(0) {}

  Order(size_t userID, size_t count, double price) : user_id(userID), count(count), price(price) {
    if (price <= 0)
      throw std::logic_error("Price can't <= 0");
    if (count == 0)
      throw std::logic_error("Count can't = 0");
  }

  bool operator>(const Order &other) const noexcept {
    return price > other.price;
  }
  bool operator==(const Order &other) const noexcept {
    return price == other.price;
  }
  bool operator<(const Order &other) const noexcept {
    return price < other.price;
  }
  bool operator!=(const Order &other) const noexcept {
    return price != other.price;
  }
  bool operator<=(const Order &other) const noexcept {
    return price <= other.price;
  }
  bool operator>=(const Order &other) const noexcept {
    return price >= other.price;
  }

  Order operator-(const Order &other) const {
    User &this_user = database.get_user(user_id);
    this_user.balance_rub += static_cast<double>(other.count) * price;
    this_user.balance_usd -= static_cast<double>(other.count);

    User &other_user = database.get_user(other.user_id);
    other_user.balance_rub -= static_cast<double>(other.count) * price;
    other_user.balance_usd += static_cast<double>(other.count);

    if (*this > other) {
      return {user_id, count - other.count, price};
    } else {
      return {user_id, 0, 0};
    }
  }
};

std::multiset<Order, std::greater<>> sell_orders{};
std::multiset<Order> buy_orders{};

#endif //TESTWORK_NTPRO_SRC_ORDER_H_
