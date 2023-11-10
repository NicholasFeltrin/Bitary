#include <QMainWindow>
#include "../forms/ui_window.h"
#include "window.h"
#include "mainTable.h"
extern "C" {
#include "src/library/library.h"
#ifdef DEBUG
#include "src/tests/tests.h"
#endif // DEBUG
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  LibraryInit();

  // Create the main table
  mainTable = ui->mainTableView;
  mainTable->showBookTable();

#ifdef DEBUG
  //startUpTest();
#endif // DEBUG
}


MainWindow::~MainWindow() {
  LibraryClose();
  delete mainTable;
  delete ui;
}


void MainWindow::on_dataBukuButton_clicked() {
  mainTable->showBookTable();
}


void MainWindow::on_dataPinjamButton_clicked() {
  mainTable->showBorrowTable();
}

void MainWindow::on_mainAddBtn_clicked() {
  mainTable->addData();
}

void MainWindow::on_searchBar_returnPressed(){
  // Get the text from the QLineEdit
  QString searchText = ui->searchBar->text();

  // Convert QString to char*
  QByteArray byteArray = searchText.toUtf8();
  const char *keyword = byteArray.constData();

  mainTable->searchData(keyword);

}

#include "window.moc"



