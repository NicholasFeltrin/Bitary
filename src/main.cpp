#include <QApplication>
#include "ui/window.h"

int main(int argc, char *argv[]){
  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
}
