#include <QMainWindow>
#include <cstdio>
#include "src/library/helper.h"
#include "window.h"
extern "C" {
#include "src/library/library.h"
}

void MainWindow::on_dataBukuButton_clicked()
{
  mainTable->showBookTable();
}


void MainWindow::on_dataPinjamButton_clicked()
{
  mainTable->showBorrowTable();
}

