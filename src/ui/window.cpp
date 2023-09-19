#include <QMainWindow>
#include <qabstractitemmodel.h>
#include <qtableview.h>
#include <QTableView>
#include <QScrollBar>
#include <qwidget.h>
#include "src/ui/models.h"
#include "ui_window.h"
#include "window.h"
#include "models.h"
extern "C" {
#include "src/library/library.h"
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  LibraryInit();

  // Init main Table
  initMainTable();
}


MainWindow::~MainWindow()
{
  LibraryClose();
  delete ui;
}

void MainWindow::initMainTable()
{

  // Create book model and borrow model
  bookModel = new BookModel(this);
  borrowModel = new BorrowModel(this);

  mainTableView = ui->mainTableView;
  mainTableView->setModel(bookModel);

  connect(mainTableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerticalScrollBarValueChanged(int)));
}

MainTableView::MainTableView(QWidget *parent)
  : QTableView(parent)
{
  int retval;
  retval = activeModel->loadBooks(LOADBEGINNING);
  CHECK(retval != 0, "Error loading books from database", return);
  mainTableView->switchModel(bookModel);
}

void MainTableView::switchModel(QAbstractTableModel *model)
{
  activeModel = model;
  setModel(model);
}
   

void MainTableView::verticalScrollBarValueChanged(int value) {
    QTableView::verticalScrollbarValueChanged(value);

    int maximumValue = verticalScrollBar()->maximum();
    int threshold = 0.9 * maximumValue;

    if (value >= threshold) {
      printf("HAMPIR COK");
    }
}


#include "window.moc"
