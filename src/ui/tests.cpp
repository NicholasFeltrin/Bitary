#include <QMainWindow>
#include "window.h"
extern "C" {
#include "src/library/library.h"
#include "src/tests/tests.h"
}

void MainWindow::on_setelanButton_clicked()
{
  TestCreateBooks(20);
}
