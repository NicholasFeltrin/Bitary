#include <QAbstractTableModel>
#include <QHeaderView>
#include <QTableView>
#include <QScrollBar>
#include <cstdio>
#include <ctime>
#include "mainTable.h"
#include "dialog.h"
extern "C"{
#include "src/library/library.h"
#include "src/library/helper.h"
}


MainTable::MainTable(QWidget *parent) {
  bookModel = new BookModel;
  borrowModel = new BorrowModel;

  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MainTable::scrollBarValueChanged);

  // Stretch Columns to fit entire table view width
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  verticalHeader()->setVisible(false);
}

void MainTable::showBookTable() {
  int retval = bookModel->loadMore(LOADBEGINNING);
  CHECK(retval != 0, "Error loading books from database", return);
  setModel(bookModel);
  activeModel = bookModel;
}

void MainTable::showBorrowTable() {
  int retval = borrowModel->loadMore(LOADBEGINNING);
  CHECK(retval != 0, "Error loading borrows from database", return);
  setModel(borrowModel);
  activeModel = borrowModel;
}

void MainTable::searchData(const char *keyword) {
  int retval = activeModel->searchData(LOADBEGINNING, keyword);
  CHECK(retval != 0, "Error loading books from database", return);
  setModel(bookModel);
  activeModel = bookModel;
}

void MainTable::loadMore() {
  activeModel->loadMore(LOADNEXT);
}

void MainTable::scrollBarValueChanged(int value) {
  QTableView::verticalScrollbarValueChanged(value);

  int maximumValue = verticalScrollBar()->maximum();
  int threshold = 0.9 * maximumValue;

  if (value >= threshold) {
    loadMore();
  }
}

void MainTable::addData() {
  activeModel->addData(this);
}

MainTable::~MainTable() {
  delete bookModel;
  delete borrowModel;
}


int BookModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return rows;
}

int BookModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant BookModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    Book book = data_[index.row()];
    switch (index.column()) {
      case 0:
        return QString(book.title);
      case 1:
        return QString(book.libraryID);
      case 2:
        if(book.borrowID != 0){
          return "Borrowed";
        }else{
          return "Avaliable";
        }
      default:
        return QVariant();
    }
  }

  return QVariant();
}

QVariant BookModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    QStringList columnNames = { "Title", "Library ID", "Status"};
    return columnNames.value(section);
  }
  return QVariant();
}

// WHAT THE FUCK
int BookModel::loadMore(Scrolling scrolling) {
  int retval = 0;
  
  retval = LibraryLoadBookChunk(&data_, scrolling);
  CHECK(retval == -1, "Error LibraryLoadBookChunk()", return -1);

  if(scrolling == LOADBEGINNING){
    beginResetModel(); 
    rows = 0;
    endResetModel();
    beginInsertRows(QModelIndex(), 0, retval); 
    rows = retval;
    endInsertRows();
  //}else if(scrolling == LOADREFRESH){
    //beginInsertRows(QModelIndex(), 0, 1); endInsertRows();
  }else if(scrolling == LOADNEXT){
    beginInsertRows(QModelIndex(), rows+1, (rows+retval)-1);
    rows += retval;
    endInsertRows();
  }
  
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
  emit dataChanged(topLeft, bottomRight);

  return 0;
}

// WHAT THE FUCK
int BookModel::searchData(Scrolling scrolling, const char *keyword) {
  int retval = 0;
  
  retval = LibrarySearchBookChunk(&data_, scrolling, keyword);
  CHECK(retval == -1, "Error LibraryLoadBookChunk()", return -1);

  if(scrolling == LOADBEGINNING){
    beginResetModel(); 
    rows = 0;
    endResetModel();
    beginInsertRows(QModelIndex(), 0, retval); 
    rows = retval;
    endInsertRows();
  //}else if(scrolling == LOADREFRESH){
    //beginInsertRows(QModelIndex(), 0, 1); endInsertRows();
  }else if(scrolling == LOADNEXT){
    beginInsertRows(QModelIndex(), rows+1, (rows+retval)-1);
    rows += retval;
    endInsertRows();
  }
  
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
  emit dataChanged(topLeft, bottomRight);

  return 0;
}

void BookModel::addData(MainTable *parent) {
  AddBookDialog *addBookDialog = new AddBookDialog(parent);
  addBookDialog->show();
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

QVariant BorrowModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole) {
    QStringList columnNames = { "Title", "Library ID", "Status"};
    return columnNames.value(section);
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

// WHAT THE FUCK
int BorrowModel::searchData(Scrolling scrolling, const char *keyword) {
  int retval = 0;
  
  retval = LibrarySearchBorrowChunk(&data_, scrolling, keyword);
  CHECK(retval == -1, "Error LibraryLoadBookChunk()", return -1);

  if(scrolling == LOADBEGINNING){
    beginResetModel(); 
    rows = 0;
    endResetModel();
    beginInsertRows(QModelIndex(), 0, retval); 
    rows = retval;
    endInsertRows();
  //}else if(scrolling == LOADREFRESH){
    //beginInsertRows(QModelIndex(), 0, 1); endInsertRows();
  }else if(scrolling == LOADNEXT){
    beginInsertRows(QModelIndex(), rows+1, (rows+retval)-1);
    rows += retval;
    endInsertRows();
  }
  
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
  emit dataChanged(topLeft, bottomRight);

  return 0;
}

void BorrowModel::addData(MainTable *parent) {
  AddBorrowDialog *addBorrowDialog = new AddBorrowDialog(parent);
  addBorrowDialog->show();
}
