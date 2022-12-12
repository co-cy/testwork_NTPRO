#include "Client.h"

// Отправка сообщения на сервер по шаблону.
void SendMessage(
    tcp::socket &aSocket,
    const std::string &message) {
  boost::asio::write(aSocket, boost::asio::buffer(message, message.size()));
}

// Возвращает строку с ответом сервера на последний запрос.
std::string ReadMessage(tcp::socket &aSocket) {
  boost::asio::streambuf b;
  boost::asio::read_until(aSocket, b, "\0");
  std::istream is(&b);
  std::string line(std::istreambuf_iterator<char>(is), {});
  return line;
}

