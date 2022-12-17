#ifndef AUTH_H
#define AUTH_H

#include <boost/asio.hpp>
#include <QMainWindow>

#include "User.h"

using boost::asio::ip::tcp;

QT_BEGIN_NAMESPACE
namespace Ui { class Auth; }
QT_END_NAMESPACE

class Auth : public QMainWindow {
 Q_OBJECT

 public:
  explicit Auth(QWidget *parent = nullptr);
  ~Auth() override;

 private:
  Ui::Auth *ui{};
  User user;
  boost::asio::io_service io_service{};
  tcp::socket sock{io_service};

 public slots:
  void authorization();
};
#endif // AUTH_H
