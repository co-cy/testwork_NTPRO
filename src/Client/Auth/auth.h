#ifndef AUTH_H
#define AUTH_H

#include <QMainWindow>

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

 public slots:
  void authorization();
};
#endif // AUTH_H
