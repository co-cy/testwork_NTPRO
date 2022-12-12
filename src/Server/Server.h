//
// Created by co_cy on 12.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_SERVER_SERVER_H_
#define TESTWORK_NTPRO_SRC_SERVER_SERVER_H_
#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include "Common.h"

using boost::asio::ip::tcp;

class Session {
 private:
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];

 public:
  explicit Session(boost::asio::io_service &io_service) : socket_(io_service), data_("") {}
  tcp::socket &socket() { return socket_; }
  void start();

 private:
  // Обработка полученного сообщения.
  void handle_read(const boost::system::error_code &error, size_t bytes_transferred);
  void handle_write(const boost::system::error_code &error);
};

class Server {
 private:
  boost::asio::io_service &io_service_;
  tcp::acceptor acceptor_;

 public:
  explicit Server(boost::asio::io_service &io_service);

 private:
  void handle_accept(Session *new_session, const boost::system::error_code &error);
};

#endif //TESTWORK_NTPRO_SRC_SERVER_SERVER_H_
