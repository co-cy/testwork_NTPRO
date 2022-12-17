#include "auth.h"

#include <QApplication>

#include <boost/system/system_error.hpp>
#include <QMessageBox>

#include <iostream>

int main(int argc, char *argv[]) {
  try {
    QApplication a(argc, argv);
    Auth w;
    w.show();
    return QApplication::exec();
  } catch (boost::system::system_error &e) {
    if (e.code() == std::errc::connection_aborted
        || e.code() == std::errc::connection_refused
        || e.code() == std::errc::connection_reset) {
      QApplication a(argc, argv);
      QMessageBox::critical(nullptr, "Ошибка", "Ошибка с подключением к серверу. Перезапустите приложение.");
      std::cerr << e.what() << e.code();
      QApplication::closeAllWindows();
      return 1;
    }
  }
}
