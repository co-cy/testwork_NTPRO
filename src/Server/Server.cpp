#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include "Common.h"
#include "Server.h"

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

  boost::asio::async_write(socket_,
                           boost::asio::buffer(data_, bytes_transferred),
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
