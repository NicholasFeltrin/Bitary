#ifndef MAINTABLE_H
#define MAINTABLE_H

#include <QAbstractTableModel>
#include <QTableView>
#include <qabstractitemmodel.h>
#include <qtableview.h>
#include <qtmetamacros.h>
extern "C" {
#include "src/library/library.h"
}

class MainTableView;
class MainModel;
class BookModel;
class BorrowModel;


class MainTable : public QTableView {
  Q_OBJECT

  public:
    MainTable(QWidget *parent = nullptr);
    ~MainTable();

    void showBookTable();
    void showBorrowTable();
    void searchData(const char *keyword);
    void addData();

  public slots:
    void scrollBarValueChanged(int value);
    void loadMore();

  private:
    MainTableView *tableView;

    MainModel *activeModel;
    BookModel *bookModel;
    BorrowModel *borrowModel;
};

class MainModel : public QAbstractTableModel {
  Q_OBJECT

  public:
    virtual int loadMore(Scrolling scrolling) = 0;
    virtual int searchData(Scrolling scrolling, const char *keyword) = 0;
    virtual void addData(MainTable *parent) = 0;

  protected:
    int rows;
    Tbuffer *data_;
};

class BookModel : public MainModel {
  Q_OBJECT

  public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int loadMore(Scrolling scrolling) override;
    void addData(MainTable *parent) override;
    int searchData(Scrolling scrolling, const char *keyword) override;

  private:
    Tbuffer *data_;
};

class BorrowModel : public MainModel {
  Q_OBJECT

  public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int loadMore(Scrolling scrolling) override;
    void addData(MainTable *parent) override;
    int searchData(Scrolling scrolling, const char *keyword) override;

  private:
    Tbuffer *data_;
};

#endif
