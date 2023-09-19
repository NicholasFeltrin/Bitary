#include <QMainWindow>
#include "src/library/helper.h"
#include "window.h"
extern "C" {
#include "src/library/library.h"
}

void MainWindow::on_loadMore_clicked()
{
  int retval;
  retval = bookModel->loadBooks(LOADBEGINNING);
  CHECK(retval != 0, "Error loading books from database", return);
}

void MainWindow::on_Books_clicked()
{
  int retval;
  retval = bookModel->loadBooks(LOADBEGINNING);
  CHECK(retval != 0, "Error loading books from database", return);
  mainTableView->switchModel(bookModel);
}


void MainWindow::on_Borrows_clicked()
{
  int retval;
  retval = borrowModel->loadBorrows(LOADBEGINNING);
  CHECK(retval != 0, "Error loading books from database", return);
  mainTableView->switchModel(borrowModel);
}

