//
// Created by co_cy on 12.12.22.
//

#include "Server.h"

int main() {
  try {
    boost::asio::io_service io_service;

    Server s(io_service);

    io_service.run();
  }
  catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}