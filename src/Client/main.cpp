//
// Created by co_cy on 12.12.22.
//

#include "Client.h"

int main() {
  try {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    s.connect(*iterator);

    while (true) {
      // Тут реализовано "бесконечное" меню.
      std::cout << "Message: ";
      std::string message;

      std::cin >> message;
      SendMessage(s, message);
      std::cout << ReadMessage(s) << std::endl;
    }
  }
  catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}