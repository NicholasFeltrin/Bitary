#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include "ui_window.h"
extern "C" {
#include "src/library/library.h"
}


class mainTableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit mainTableModel(QObject *parent = nullptr);
  ~mainTableModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  void loadBooks(Book **data);

private:
  Book *data_;
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_loadMore_clicked();
    void on_loadMore_pressed();

private:
    Ui::MainWindow *ui;
    mainTableModel *bookModel;
};


#endif // WINDOW_H
