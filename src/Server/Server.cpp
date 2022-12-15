#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <set>

#include "Response/Response.h"
#include "Request/Request.h"
#include "UserBase.h"
#include "Common.h"
#include "Server.h"
#include "Order.h"

using boost::asio::ip::tcp;

void Session::start() {
  socket_.async_read_some(boost::asio::buffer(data_, max_length),
                          boost::bind(&Session::handle_read, this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

// Обработка полученного сообщения.
void Session::handle_read(const boost::system::error_code &error, size_t bytes_transferred) {
  if (error) {
    delete this;
    return;
  }

  data_[bytes_transferred] = '\0';
  std::istringstream iss(data_);

  std::cout << "Get data: _" << data_ << "_\n";

  char type_command;
  iss >> type_command;

  std::cout << "Sell size: " << sell_orders.size() << "\n";
  for (const auto &sell_order : sell_orders) {
    std::cout << "Sell order: " << std::to_string(sell_order.user_id) << " " << std::to_string(sell_order.count) << " "
              << std::to_string(sell_order.price) << " " << "\n";
  }

  std::cout << "Buy size: " << buy_orders.size() << "\n";
  for (const auto &buy_order : buy_orders) {
    std::cout << "Buy order: " << std::to_string(buy_order.user_id) << " " << std::to_string(buy_order.count) << " "
              << std::to_string(buy_order.price) << " " << "\n";
  }

  std::string answer = std::to_string(Response::TypeInvalid);

  if (type_command == Request::TypeRegistration) {
    // Go to registration handler
    Response::BoolMessage response(false);
    Request::Registration reg_message{iss};

    if (database.find(reg_message.login)) {
      response.message = "Login already used";
    } else {
      size_t user_id = database.append(reg_message.login, reg_message.password);

      response.message = std::to_string(user_id);
      response.state = true;
    }

    answer = response;
  } else if (type_command == Request::TypeAuth) {
    // Go to auth handler
    Response::BoolMessage response(false);
    Request::Auth auth_message{iss};

    if (!database.find(auth_message.login)) {
      response.message = "Login or password bad";
    } else {
      User &user = database.get_user(auth_message.login);

      if (user.password != auth_message.password) {
        response.message = "Login or password bad";
      } else {
        response.message = std::to_string(user.user_id);
        response.state = true;
      }
    }

    answer = response;
  } else if (type_command == Request::TypeCreateOrder) {
    Response::BoolMessage response(false);
    Request::CreateOrder order{iss};

    if (order.is_buy) {
      for (auto iter = sell_orders.begin(); iter != sell_orders.end() && !order.order.empty();) {
        if (iter->user_id != order.order.user_id) {
          if (*iter > order.order) {
            auto half_order = sell_orders.extract(iter).value();
            half_order.sub(order.order, order.is_buy);
            sell_orders.insert(half_order);
          } else {
            order.order.sub(*iter, !order.is_buy);
            iter = sell_orders.erase(iter);
            continue;
          }
        }
        ++iter;
      }

      if (!order.order.empty())
        buy_orders.insert(order.order);
    } else {
      for (auto iter = buy_orders.begin(); iter != buy_orders.end() && !order.order.empty();) {
        if (iter->user_id != order.order.user_id) {
          if (*iter > order.order) {
            auto half_order = buy_orders.extract(iter).value();
            half_order.sub(order.order, order.is_buy);
            buy_orders.insert(half_order);
          } else {
            order.order.sub(*iter, !order.is_buy);
            iter = buy_orders.erase(iter);
            continue;
          }
        }
        ++iter;
      }

      if (!order.order.empty())
        sell_orders.insert(order.order);
    }

    response.state = true;
    response.message = "Good";
    answer = response;
  } else if (type_command == Request::TypeInfoUser) {
    Request::InfoUser info{iss};
    if (database.find(info.user_id)) {
      answer = Response::InfoUser(database.get_user(info.user_id));
    } else {
      answer = Response::BoolMessage(false, "User not found!");
    }
  } else if (type_command == Request::TypeSellerOrders) {
  } else if (type_command == Request::TypeBuyOrders) {
  }

  boost::asio::async_write(socket_,
                           boost::asio::buffer(answer, answer.size()),
                           boost::bind(&Session::handle_write, this,
                                       boost::asio::placeholders::error));
}

void Session::handle_write(const boost::system::error_code &error) {
  if (error) {
    delete this;
    return;
  }
  socket_.async_read_some(boost::asio::buffer(data_, max_length),
                          boost::bind(&Session::handle_read, this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

Server::Server(boost::asio::io_service &io_service)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "Server started! Listen " << port << " port" << std::endl;

  auto *new_session = new Session(io_service_);
  acceptor_.async_accept(new_session->socket(),
                         boost::bind(&Server::handle_accept, this, new_session,
                                     boost::asio::placeholders::error));
}

void Server::handle_accept(Session *new_session,
                           const boost::system::error_code &error) {
  if (!error) {
    new_session->start();
    new_session = new Session(io_service_);
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&Server::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
  } else {
    delete new_session;
  }
}
