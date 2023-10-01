#include <QAbstractTableModel>
#include <QTableView>
#include <QScrollBar>
#include <cstdio>
#include <ctime>
#include <qabstractitemmodel.h>
#include <qtableview.h>
#include "mainTable.h"
#include "src/ui/window.h"
extern "C"{
#include "src/library/library.h"
#include "src/library/helper.h"
}


MainTable::MainTable(MainTableView &uiTableView) {
  tableView = &uiTableView;

  bookModel = new BookModel;
  borrowModel = new BorrowModel;
}

MainTable::~MainTable() {
  delete bookModel;
  delete borrowModel;
}

MainTableView::MainTableView(QWidget *parent) : QTableView(parent) {
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MainTableView::scrollBarValueChanged);
}

void MainTable::showBookTable() {
  int retval = bookModel->loadMore(LOADBEGINNING);
  CHECK(retval != 0, "Error loading books from database", return);
  tableView->setModel(bookModel);
  //activeModel = bookModel;
}

void MainTable::showBorrowTable() {
  int retval = borrowModel->loadMore(LOADBEGINNING);
  CHECK(retval != 0, "Error loading borrows from database", return);
  tableView->setModel(borrowModel);
  //activeModel = borrowModel;
}

void MainTableView::scrollBarValueChanged(int value) {
  QTableView::verticalScrollbarValueChanged(value);

  int maximumValue = verticalScrollBar()->maximum();
  int threshold = 0.9 * maximumValue;

  if (value >= threshold) {
    //->
    //mainTable->activeModel->loadMore(LOADNEXT) ;
  }
}


int BookModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return rows;
}

int BookModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 4;
}

QVariant BookModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    Book book = data_[index.row()];
    switch (index.column()) {
      case 0:
        return book.bookID;
      case 1:
        return book.borrowID;
      case 2:
        return QString(book.title);
      case 3:
        return QString(book.libraryID);
      default:
        return QVariant();
    }
  }

  return QVariant();
}

int BookModel::loadMore(Scrolling scrolling) {
  int retval = 0;
  
  retval = LibraryLoadBookChunk(&data_, scrolling);
  CHECK(retval == -1, "Error LibraryLoadBookChunk()", return -1);

  if(scrolling == LOADBEGINNING && rows < retval){
    beginInsertRows(QModelIndex(), 0, (retval-rows)-1); endInsertRows();
    rows = retval;
  }else if(scrolling == LOADBEGINNING && rows > retval){
    beginRemoveRows(QModelIndex(), rows-(rows-retval), rows-1);
    rows = retval;
  }else if(scrolling == LOADNEXT){
    beginInsertRows(QModelIndex(), retval-rows, (rows+retval)-1); endInsertRows();
    rows += retval;
  }
  
  // refresh not needed 
  //QModelIndex topLeft = createIndex(0, 0);
  //QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
  //emit dataChanged(topLeft, bottomRight);

  return 0;
}


int BorrowModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return rows;
}

int BorrowModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 6;
}

QVariant BorrowModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    Borrow borrow = data_[index.row()];
    switch (index.column()) {
      case 0:
        return borrow.borrowID;
      case 1:
        return borrow.bookID;
      case 2:
        return QString(asctime(localtime(&borrow.startTimestamp)));
      case 3:
        return QString(asctime(localtime(&borrow.endTimestamp)));
      case 4:
        return QString(borrow.name);
      case 5:
        return QString(borrow.classSequence);
      default:
        return QVariant();
    }
  }

  return QVariant();
}

int BorrowModel::loadMore(Scrolling scrolling) {
  int retval = 0;

  retval = LibraryLoadBorrowChunk(&data_, scrolling);
  CHECK(retval == -1, "Error LibraryLoadBorrowChunk()", return -1);

  if(scrolling == LOADBEGINNING && rows < retval){
    beginInsertRows(QModelIndex(), 0, (retval-rows)-1); endInsertRows();
    rows = retval;
  }else if(scrolling == LOADBEGINNING && rows > retval){
    beginRemoveRows(QModelIndex(), rows-(rows-retval), rows-1);
    rows = retval;
  }
  
  // refresh not needed 
  //QModelIndex topLeft = createIndex(0, 0);
  //QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
  //emit dataChanged(topLeft, bottomRight);

  return 0;
}
