#include "trade.h"

#include <utility>
#include <QMessageBox>

#include "./ui_trade.h"

#include "Response/Response.h"
#include "Request/Request.h"

#include "Helpers.h"

Trade::Trade(QWidget *parent, User user, tcp::socket *sock)
    : QMainWindow(parent), ui(new Ui::Trade), user(std::move(user)), sock(sock) {
  ui->setupUi(this);

  connect(ui->submit, &QPushButton::clicked, this, &Trade::send_order);
  connect(ui->update, &QPushButton::clicked, this, &Trade::update_info);

  update_info();
}

Trade::~Trade() {
  delete ui;
}

void Trade::send_order() {
  SendMessage(*sock,
              Request::CreateOrder(user.user_id, ui->isBuy->isChecked(), ui->count->value(), ui->price->value()));
  auto stream = ReadMessage(*sock);
  char type_response = Response::TypeInvalid;
  stream >> type_response;
  if (type_response == Response::TypeBoolMessage) {
    Response::BoolMessage res{stream};
    if (res.state) {
      QMessageBox::information(this, "Успешно", "Заявка успешно принята");
    } else {
      QMessageBox::warning(this, "Ошибка", QString::fromStdString(res.message));
    }
  } else {
    QMessageBox::warning(this, "Ошибка", "Не верный формат ответа. Свяжитесь с адм.");
  }

  update_info();
}

void Trade::refresh_seller() {
  ui->seller->clear();

  SendMessage(*sock, Request::InfoSellerOrders(user.user_id));
  auto stream = ReadMessage(*sock);

  char type_response = Response::TypeInvalid;
  stream >> type_response;
  if (type_response == Response::TypeInfoSellerOrders) {
    Response::InfoSellerOrders info{stream};
    for (const auto &kOrder : info.orders) {
      auto *item = new QListWidgetItem(QString::fromStdString(
          "Price: " + std::to_string(kOrder.price) + "; Count: " + std::to_string(kOrder.count)));
      if (kOrder.user_id == user.user_id)
        item->setBackground(QBrush({55, 55, 255, 50}));
      ui->seller->addItem(item);
    }
  } else {
    QMessageBox::critical(this, "Ошибка", "Не удалось обновить заявки");
  }
}
void Trade::refresh_buyer() {
  ui->buyer->clear();

  SendMessage(*sock, Request::InfoBuyOrders(user.user_id));
  auto stream = ReadMessage(*sock);

  char type_response = Response::TypeInvalid;
  stream >> type_response;
  if (type_response == Response::TypeInfoBuyOrders) {
    Response::InfoBuyOrders info{stream};
    for (const auto &kOrder : info.orders) {
      auto *item = new QListWidgetItem(QString::fromStdString(
          "Price: " + std::to_string(kOrder.price) + "; Count: " + std::to_string(kOrder.count)));
      if (kOrder.user_id == user.user_id)
        item->setBackground(QBrush({55, 55, 255, 50}));
      ui->buyer->addItem(item);
    }
  } else {
    QMessageBox::critical(this, "Ошибка", "Не удалось обновить заявки");
  }
}

void Trade::update_user_info() {
  SendMessage(*sock, Request::InfoUser(this->user.user_id));
  auto stream = ReadMessage(*sock);

  char type_response = Response::TypeInvalid;
  stream >> type_response;
  if (type_response == Response::TypeInfoUser) {
    Response::InfoUser info{stream};

    ui->rubValue->setText(QString::number(info.balance_rub));
    ui->usdValue->setText(QString::number(info.balance_usd));
  } else {
    QMessageBox::critical(this, "Ошибка", "Не удалось обновить баланс пользователя");
  }
}

void Trade::update_info() {
  refresh_seller();
  refresh_buyer();
  update_user_info();
}
