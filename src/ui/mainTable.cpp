#include <QAbstractTableModel>
#include <ctime>
#include "models.h"
#include "src/library/helper.h"
extern "C"{
#include "src/library/library.h"
}

BookModel::BookModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  LibraryLoadBookChunk(&dataBookBuffer, LOADBEGINNING);
  data_ = dataBookBuffer;
}

BookModel::~BookModel()
{
  // Close
}

int BookModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return BOOK_CHUNK;
}

int BookModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 4;
}

QVariant BookModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    Book book = data_[index.row()];
    switch (index.column()) {
      case 0:
        return book.bookID;
      case 1:
        return book.borrowID;
      case 2:
        return book.title;
      case 3:
        return book.libraryID;
      default:
        return QVariant();
    }
  }

  return QVariant();
}

int BookModel::loadBooks(Scrolling scrolling)
{
  int retval = 0;
    
  beginInsertRows(QModelIndex(), 0, BOOK_CHUNK - 1);
  retval = LibraryLoadBookChunk(&data_, scrolling);
  CHECK(retval != 0, "Error LibraryLoadBookChunk()", return -1);
  endInsertRows();

  return 0;
}


BorrowModel::BorrowModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  LibraryLoadBorrowChunk(&dataBorrowBuffer, LOADBEGINNING);
  data_ = dataBorrowBuffer;
}

BorrowModel::~BorrowModel()
{
  // Close
}

int BorrowModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return BORROW_CHUNK;
}

int BorrowModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 6;
}

QVariant BorrowModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    Borrow borrow = data_[index.row()];
    switch (index.column()) {
      case 0:
        return borrow.borrowID;
      case 1:
        return borrow.bookID;
      case 2:
        return asctime(localtime(&borrow.startTimestamp));
      case 3:
        return asctime(localtime(&borrow.endTimestamp));
      case 4:
        return borrow.name;
      case 5:
        return borrow.classSequence;
      default:
        return QVariant();
    }
  }

  return QVariant();
}

int BorrowModel::loadBorrows(Scrolling scrolling)
{
  int retval = 0;

  beginInsertRows(QModelIndex(), 0, BORROW_CHUNK - 1);
  retval = LibraryLoadBorrowChunk(&data_, scrolling);
  CHECK(retval != 0, "Error LibraryLoadBorrowChunk()", return -1);
  endInsertRows();

  return 0;
}

