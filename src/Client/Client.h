//
// Created by co_cy on 12.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_CLIENT_CLIENT_H_
#define TESTWORK_NTPRO_SRC_CLIENT_CLIENT_H_

#include <iostream>
#include <boost/asio.hpp>

#include "Common.h"

using boost::asio::ip::tcp;

// Отправка сообщения на сервер по шаблону.
void SendMessage(
    tcp::socket &aSocket,
    const std::string &message);

// Возвращает строку с ответом сервера на последний запрос.
std::string ReadMessage(tcp::socket &aSocket);

#endif //TESTWORK_NTPRO_SRC_CLIENT_CLIENT_H_
