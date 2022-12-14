#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <set>

#include "Response/Response.h"
#include "Request/Request.h"
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

  char type_command;
  iss >> type_command;

  Response::BoolMessage response(false);

  if (type_command == Request::TypeRegistration) {
    // Go to registration handler
    Request::Registration reg_message{iss};
    if (database.find(reg_message.login)) {
      response.message = "Login already used";
    } else {
      size_t user_id = database.append(reg_message.login, reg_message.password);

      response.message = std::to_string(user_id);
      response.state = true;
    }
  } else if (type_command == Request::TypeAuth) {
    // Go to auth handler
    Request::Auth auth_message{iss};
    if (!database.find(auth_message.login)) {
      response.message = "Login or password bad";
    } else {
      User &user = database.get_user(auth_message.login);

      if (user.password != auth_message.password) {
        response.message = "Login or password bad";
      } else {
        response.message = "Auth\n" + std::to_string(user.user_id);
        response.state = true;
      }
    }
  } else {
    // Go to 404 handler
    response.message = "Endpoint not found";
  }

  std::string message = std::string(response);

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
