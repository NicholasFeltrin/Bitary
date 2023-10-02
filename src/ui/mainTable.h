#ifndef MAINTABLE_H
#define MAINTABLE_H

#include <QAbstractTableModel>
#include <QTableView>
extern "C" {
#include "src/library/library.h"
}

class MainTableView;
class MainModel;
class BookModel;
class BorrowModel;


class MainTable {
  public:
    MainTable(MainTableView &uiTableView);
    ~MainTable();

    void showBookTable();
    void showBorrowTable();

  private:
    MainTableView *tableView;

    MainModel *activeModel;
    BookModel *bookModel;
    BorrowModel *borrowModel;
};

class MainTableView : public QTableView  {
  Q_OBJECT

  public:
    MainTableView(QWidget *parent = nullptr);

  public slots:
    void scrollBarValueChanged(int value);
};

class MainModel : public QAbstractTableModel {
  Q_OBJECT

  public:
    //virtual int loadMore(Scrolling scrolling);

  protected:
    int rows;
    void *data_;
};

class BookModel : public MainModel {
  Q_OBJECT

  public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role);
    int loadMore(Scrolling scrolling);

  private:
    Book *data_;
};

class BorrowModel : public MainModel {
  Q_OBJECT

  public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role);
    int loadMore(Scrolling scrolling);

  private:
    Borrow *data_;
};


#endif
