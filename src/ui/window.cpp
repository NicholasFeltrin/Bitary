#include <QMainWindow>
#include "ui_window.h"
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

#ifdef DEBUG
  startUpTest();
#endif // DEBUG
}


MainWindow::~MainWindow() {
  LibraryClose();
  delete mainTable;
  delete ui;
}


#include "window.moc"
