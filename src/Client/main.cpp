//
// Created by co_cy on 12.12.22.
//

#include "Client.h"
#include "Request/Request.h"
#include "Response/Response.h"

#include "User.h"

int main() {
  User user;

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
                   "3) Create order\n"
                   "4) Info user\n"
                   "5) Exit\n"
                << std::endl;

      short menu_option_num;
      std::cin >> menu_option_num;
      if (menu_option_num == 1) {
        std::string login;
        std::string password;

        std::cout << "login: ";
        std::cin >> login;
        std::cout << "\npassword: ";
        std::cin >> password;
        SendMessage(s, Request::Registration(login, password));
        auto stream = ReadMessage(s);

        char type_answer = Response::TypeInvalid;
        stream >> type_answer;
        if (type_answer == Response::TypeBoolMessage) {
          Response::BoolMessage response(stream);
          if (response.state) {
            std::cout << "Registration result: " << response.state << "\nMessage: " << response.message << "\n";
            user = User(std::stol(response.message), login, password);
            std::cout << std::string(user) << "\n";
          } else {
            std::cout << "Bad. Message: " << response.message << "\n";
          }
        } else {
          std::cout << "Bad type answer: " << int(type_answer) << "\n";
        }
      } else if (menu_option_num == 2) {
        std::string login;
        std::string password;

        std::cout << "login: ";
        std::cin >> login;
        std::cout << "\npassword: ";
        std::cin >> password;

        SendMessage(s, Request::Auth(login, password));
        auto stream = ReadMessage(s);

        char type_answer = Response::TypeInvalid;
        stream >> type_answer;
        if (type_answer == Response::TypeBoolMessage) {
          Response::BoolMessage response(stream);
          if (response.state) {
            std::cout << "Auth result: " << response.state << "\nMessage: " << response.message << "\n";
            user = User(std::stol(response.message), login, password);
            std::cout << std::string(user) << "\n";
          } else {
            std::cout << "Bad. Message: " << response.message << "\n";
          }
        } else {
          std::cout << "Bad type answer: " << int(type_answer);
        }
      } else if (menu_option_num == 3) {
        if (user.user_id) {
          bool is_buy;
          size_t count;
          double price;

          std::cout << "Is buy: ";
          std::cin >> is_buy;
          std::cout << "count: ";
          std::cin >> count;
          std::cout << "price: ";
          std::cin >> price;

          SendMessage(s, Request::CreateOrder(is_buy, user.user_id, count, price));
          auto stream = ReadMessage(s);

          char type_answer = Response::TypeInvalid;
          stream >> type_answer;
          if (type_answer == Response::TypeBoolMessage) {
            Response::BoolMessage response(stream);
            if (response.state) {
              std::cout << "Order result: " << response.state << "\n";
            } else {
              std::cout << "Bad. Message: " << response.message << "\n";
            }
          } else {
            std::cout << "Bad type answer: " << int(type_answer) << "\n";
          }

        } else {
          std::cout << "You aren't registered or auth!\n";
        }
      } else if (menu_option_num == 4) {
        if (user.user_id) {
          SendMessage(s, Request::InfoUser(user.user_id));
          auto stream = ReadMessage(s);

          char type_answer = Response::TypeInvalid;
          stream >> type_answer;
          if (type_answer == Response::TypeInfoUser) {
            std::cout << std::string(Response::InfoUser(stream)) << "\n";
          } else {
            std::cout << "Bad type answer: " << int(type_answer) << "\n";
          }

        } else {
          std::cout << "You aren't registered or auth\n!";
        }
      } else if (menu_option_num == 5) {
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