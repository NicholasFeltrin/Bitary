#include <QMainWindow>
#include <cstdio>
#include "src/library/helper.h"
#include "window.h"
extern "C" {
#include "src/library/library.h"
}

void MainWindow::on_Books_clicked()
{
  mainTable->showBookTable();
}


void MainWindow::on_Borrows_clicked()
{
  mainTable->showBorrowTable();
}

