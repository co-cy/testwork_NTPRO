#include <QMessageBox>
#include <iostream>

#include "./ui_auth.h"
#include "auth.h"

#include "Response/Response.h"
#include "Request/Request.h"

#include "Helpers.h"

#include "Common.h"

Auth::Auth(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Auth) {
  ui->setupUi(this);

  connect(ui->button, &QPushButton::released, this, &Auth::authorization);

  tcp::resolver resolver(io_service);
  tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
  tcp::resolver::iterator iterator = resolver.resolve(query);

  sock.connect(*iterator);
}

Auth::~Auth() {
  delete ui;
}

void Auth::authorization() {
  if (this->ui->login->text().isEmpty() || this->ui->password->text().isEmpty()) {
    this->ui->errorsLabel->setText("Login or password is not set");
    return;
  }

  if (ui->checkBox->isChecked())
    SendMessage(sock, Request::Registration(ui->login->text().toStdString(), ui->password->text().toStdString()));
  else
    SendMessage(sock, Request::Auth(ui->login->text().toStdString(), ui->password->text().toStdString()));
  auto stream = ReadMessage(sock);

  char type_response = Response::TypeInvalid;
  stream >> type_response;
  if (type_response == Response::TypeBoolMessage) {
    Response::BoolMessage res{stream};
    if (res.state) {
      user = User(std::stol(res.message), ui->login->text().toStdString(), ui->password->text().toStdString());
      ui->errorsLabel->setText(QString::fromStdString(std::string(user)));
    } else {
      ui->errorsLabel->setText(QString::fromStdString(res.message));
    }
  } else {
    ui->errorsLabel->setText("Ошибка запроса свяжитесь с администратором");
  }
}

