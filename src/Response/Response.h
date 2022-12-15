//
// Created by co_cy on 12.12.22.
//

#ifndef TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_
#define TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_

#include <string>
#include <sstream>

namespace Response {

enum TypeRequest : char {
  TypeInvalid = 0,
  TypeBoolMessage = 1,

};

struct BoolMessage {
  bool state;
  std::string message;

  explicit BoolMessage(bool state) : state(state) {};
  BoolMessage(bool state, std::string &comment) : state(state), message(comment) {};
  explicit BoolMessage(std::istringstream &stream) : state() {
    stream >> state;
    getline(stream, message, '\0');
  };

  operator std::string() const {
    return std::string(1, TypeBoolMessage) + " " + std::to_string(state) + " " + message;
  }
};

}

#endif //TESTWORK_NTPRO_SRC_RESPONSE_RESPONSE_H_
