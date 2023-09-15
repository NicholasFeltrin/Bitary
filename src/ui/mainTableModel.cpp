#include "mainTableModel.h"

mainTableModel::mainTableModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  // Init
}

mainTableModel::~mainTableModel()
{
  // Close
}

int mainTableModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return BOOK_CHUNK;
}

int mainTableModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 4;
}

QVariant mainTableModel::data(const QModelIndex &index, int role) const
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
        return 69;
      default:
        return QVariant();
    }
  }

  return QVariant();
}

void mainTableModel::loadBooks(Book **data)
{
  beginInsertRows(QModelIndex(), 0, BOOK_CHUNK - 1);
  data_ = *data;
  endInsertRows();
}

