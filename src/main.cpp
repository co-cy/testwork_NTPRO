//
// Created by co_cy on 11.12.22.
//


#include <iostream>
#include "Order.h"

int main() {

  std::priority_queue<Order> order_buy;
  std::priority_queue<Order, std::vector<Order>, std::greater<>> gora;
  Order a(0, 10, 10);
  Order b(1, 10, 10);
  Order c(2, 10, 10);
  gora.push(a);
  gora.push(b);
  gora.push(c);
  std::cout << gora.size();
  std::cout << gora.top().userID;

  return 0;
}