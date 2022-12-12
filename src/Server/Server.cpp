#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <queue>

#include "Response/Response.h"
#include "Request/Request.h"
#include "Common.h"
#include "Server.h"
#include "Order.h"

using boost::asio::ip::tcp;

std::priority_queue<Order, std::vector<Order>, std::greater<>> sell_orders{};
std::priority_queue<Order> buy_orders{};
UserBase database{};

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
  std::cout << "Data command: " << data_ << "\n";
  std::istringstream iss(data_);

  char type_command;
  iss >> type_command;
  std::cout << "Type command: " << int(type_command) << "\n";

  Response::BoolMessage response(true);
  std::string message;

  if (type_command == Request::TypeRegistration) {
    Request::Registration reg_message{iss};
    if (database.find(reg_message.user.login)) {
      message = "Login already used";
      response.state = false;
    } else {
      database.append(reg_message.user);
      message = "Registration:\n" + std::string(reg_message.user) + "\n";
    }
  } else if (type_command == Request::TypeAuth) {
    Request::Auth auth_message{iss};
    if (!database.find(auth_message.user)) {
      message = "Login or password bad";
      response.state = false;
    } else {
      message = "Auth\n" + std::string(auth_message.user) + "\n";
    }
  }

  std::cout << message;

  response.message = std::move(message);
  message = std::string(response);

  boost::asio::async_write(socket_,
                           boost::asio::buffer(message, message.size()),
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
