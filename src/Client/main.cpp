//
// Created by co_cy on 12.12.22.
//

#include "Client.h"
#include "Request/Request.h"
#include "Response/Response.h"

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
      std::cout << "Menu:\n"
                   "1) Registration\n"
                   "2) Auth\n"
                   "3) Exit\n"
                << std::endl;

      short menu_option_num;
      std::cin >> menu_option_num;
      if (menu_option_num == 1) {
        std::string login;
        std::string password;

        std::cin >> login >> password;

        SendMessage(s, Request::Registration(login, password));
        auto stream = ReadMessage(s);

        char type_answer;
        stream >> type_answer;
        if (type_answer == Response::TypeBoolMessage) {
          Response::BoolMessage response(stream);
          std::cout << "Registration result: " << response.state << "\nMessage: " << response.message;
        } else {
          std::cout << "Bad type answer: " << int(type_answer);
        }
      } else if (menu_option_num == 2) {
        std::string login;
        std::string password;

        std::cin >> login >> password;

        SendMessage(s, Request::Auth(login, password));
        auto stream = ReadMessage(s);

        char type_answer;
        stream >> type_answer;
        if (type_answer == Response::TypeBoolMessage) {
          Response::BoolMessage response(stream);
          std::cout << "Auth result: " << response.state << "\nMessage: " << response.message;
        } else {
          std::cout << "Bad type answer: " << int(type_answer);
        }
      } else if (menu_option_num == 3) {
        exit(0);
      } else {
        std::cout << "Unknown menu option\n" << std::endl;
        break;
      }
    }
  }
  catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}