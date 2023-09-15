#include <QAbstractTableModel>
#include <qabstractitemmodel.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include "window.h"

#ifdef DEBUG
#include <QDebug>
#endif // DEBUG

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  bookModel = new mainTableModel(this);

  ui->mainTableView->setModel(bookModel);
  loadBookDataChunk(&dataBookBuffer, LOADBEGINNING);
  bookModel->loadBooks(&dataBookBuffer);
}


MainWindow::~MainWindow()
{
  closeLibrary();
  delete ui;
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
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        Book book = data_[index.row()];
        switch (index.column()) {
            case 0:
              //printf("%d\n", data_[0].bookID);
                return book.bookID;
            case 1:
              //printf("%d\n", book.borrowID);
                return book.borrowID;
            case 2:
              //printf("%p\n", book.title);
                return book.title;
            case 3:
                //return book.libraryID;
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

mainTableModel::mainTableModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //data_ = (Book*)malloc(sizeof(Book)*BOOK_CHUNK);
}


mainTableModel::~mainTableModel()
{
}

void MainWindow::on_loadMore_clicked()
{
  loadBookDataChunk(&dataBookBuffer, LOADBEGINNING);
  bookModel->loadBooks(&dataBookBuffer);
}

#include "window.moc"
