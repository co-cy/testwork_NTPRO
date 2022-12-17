#ifndef TRADE_H
#define TRADE_H

#include <boost/asio.hpp>
#include <QMainWindow>

#include "User.h"

using boost::asio::ip::tcp;

QT_BEGIN_NAMESPACE
namespace Ui { class Trade; }
QT_END_NAMESPACE

class Trade : public QMainWindow {
 Q_OBJECT

 public:
  explicit Trade(QWidget *parent, User user, tcp::socket *sock);
  ~Trade() override;

 private:
  Ui::Trade *ui;
  User user;
  tcp::socket *sock;
  void refresh_seller();
  void refresh_buyer();
  void update_user_info();

 public slots:
  void send_order();
  void update_info();
};
#endif // TRADE_H
