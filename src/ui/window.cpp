#include <QMainWindow>
#include "ui_window.h"
#include "window.h"
#include "mainTableModel.h"
extern "C" {
#include "src/library/library.h"
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  initLibrary();

  //createDatabase();
  //test();
  //testFetch();
  //closeLibrary();

  // Init main Table
  bookModel = new mainTableModel(this);
  ui->mainTableView->setModel(bookModel);
  // Load database data into table view
  loadBookDataChunk(&dataBookBuffer, LOADBEGINNING);
  bookModel->loadBooks(&dataBookBuffer);
}


MainWindow::~MainWindow()
{
  closeLibrary();
  delete ui;
}

#include "window.moc"
