//
// Created by co_cy on 17.12.22.
//

#include "Helpers.h"

// Отправка сообщения на сервер по шаблону.
void SendMessage(
    tcp::socket &aSocket,
    const std::string &message) {
//  std::cout << "Send message: _" << message << "_\n";
  boost::asio::write(aSocket, boost::asio::buffer(message, message.size()));
}

// Возвращает строку с ответом сервера на последний запрос.
std::istringstream ReadMessage(tcp::socket &aSocket) {
  boost::asio::streambuf b;
  boost::asio::read_until(aSocket, b, "\0");
  std::istream is(&b);
  std::string line(std::istreambuf_iterator<char>(is), {});
  return std::istringstream(line);
}