#include "auth.h"
#include "./ui_auth.h"

#include "Response/Response.h"
#include "Request/Request.h"

Auth::Auth(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Auth) {
  ui->setupUi(this);

  connect(ui->button, &QPushButton::released, this, &Auth::authorization);
}

Auth::~Auth() {
  delete ui;
}

void Auth::authorization() {
  if (this->ui->login->text().isEmpty() || this->ui->password->text().isEmpty()) {
    this->ui->errorsLabel->setText("Login or password is not set");
    return;
  }

}

