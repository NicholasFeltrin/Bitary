#include <QApplication>
#include "ui/window.h"
extern "C" {
#include "library/library.h"
}

int main(int argc, char *argv[]){
  initLibrary();
  //createDatabase();
  //test();
  //testFetch();
  //closeLibrary()s

  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
}
