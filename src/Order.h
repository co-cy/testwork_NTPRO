//
// Created by co_cy on 11.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_ORDER_H_
#define TESTWORK_NTPRO_SRC_ORDER_H_

#include <stdexcept>
#include <queue>
#include <vector>

struct Order {
  std::size_t userID;
  std::size_t count;
  double price;

  Order(std::size_t userID, std::size_t count, double price) : userID(userID), count(count), price(price) {
    if (price <= 0)
      throw std::logic_error("Price can't <= 0");
    if (count == 0)
      throw std::logic_error("Count can't = 0");
  };

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
};

#endif //TESTWORK_NTPRO_SRC_ORDER_H_
