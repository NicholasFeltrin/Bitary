#ifndef MODELS_H
#define MODELS_H

#include <QAbstractTableModel>
#include <qabstractitemmodel.h>
#include <qobjectdefs.h>
#include <qwidget.h>
#include <vector>
extern "C" {
#include "src/library/library.h"
}

class BookModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit BookModel(QObject *parent = nullptr);
  ~BookModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  int loadMore(Scrolling scrolling);

private:
  Book *data_;
};

class BorrowModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit BorrowModel(QObject *parent = nullptr);
  ~BorrowModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  int loadMore(Scrolling scrolling);

private:
  Borrow *data_;
};

class MainTableView : public QTableView
{
  Q_OBJECT
  
public:
  MainTableView(QWidget *parent = nullptr);

protected:
 void scrollContentsBy(int dx, int dy) override;

private:
  QAbstractTableModel *activeModel;
  BookModel bookmodel;
  BorrowModel borrowModel;
};


#endif // !MODELS_H
