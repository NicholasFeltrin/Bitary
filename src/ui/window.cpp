#include <QMainWindow>
#include "ui_window.h"
#include "window.h"
#include "mainTable.h"
extern "C" {
#include "src/library/library.h"
#include "src/tests/tests.h"
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  LibraryInit();

  // Create the main table
  mainTable = new MainTable(*ui->mainTableView);
}


MainWindow::~MainWindow() {
  LibraryClose();
  delete mainTable;
  delete ui;
}


#include "window.moc"
