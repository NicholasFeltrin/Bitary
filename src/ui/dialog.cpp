#include <QMainWindow>
#include <QLineEdit>
#include <cstdio>
#include <time.h>
#include <qdialog.h>
#include "dialog.h"
#include "../forms/ui_addBookPopUpWindow.h"
#include "../forms/ui_addBorrowPopUpWindow.h"
extern "C"{
#include "src/library/library.h"
#include "src/library/helper.h"
}

AddBookDialog::AddBookDialog(MainTable *parent) : QDialog(parent), ui(new Ui::AddBookDialog) {
  ui->setupUi(this);

  mainTableParent = parent;
  connect(ui->titleField, &QLineEdit::textChanged, this, &AddBookDialog::onTitleFieldTextChanged);
  connect(ui->libraryIDField, &QLineEdit::textChanged, this, &AddBookDialog::onLibraryIDFieldChanged);
}

AddBookDialog::~AddBookDialog() {
  delete ui;
}

void AddBookDialog::onTitleFieldTextChanged(const QString &input) {
  bookToAdd.title = strdup(input.toLocal8Bit().data());
}

void AddBookDialog::onLibraryIDFieldChanged(const QString &input) {
  bookToAdd.libraryID = strdup(input.toLocal8Bit().data());
}

void AddBookDialog::on_confirmAdd_clicked() {
  // Add Book
  bookToAdd.borrowID = 0;
  LibraryCreateBook(&bookToAdd);

  // Update Table
  mainTableParent->showBookTable();

  free(bookToAdd.title);
  free(bookToAdd.libraryID);
  accept();
}


AddBorrowDialog::AddBorrowDialog(MainTable *parent) : QDialog(parent), ui(new Ui::AddBorrowDialog) {
  ui->setupUi(this);

  mainTableParent = parent;
  connect(ui->nameField, &QLineEdit::textChanged, this, &AddBorrowDialog::onNameFieldTextChanged);
  connect(ui->classSequenceField, &QLineEdit::textChanged, this, &AddBorrowDialog::onClassSequenceFieldTextChanged);

  bookListModel = new BookListModel;
  ui->searchBox->setModel(bookListModel);

  int retval = bookListModel->loadMore(LOADBEGINNING);
  CHECK(retval != 0, "Error loading books from database", return);
}

AddBorrowDialog::~AddBorrowDialog() {
  delete bookListModel;
  delete ui;
}

void AddBorrowDialog::onNameFieldTextChanged(const QString &input) {
  borrowToAdd.name = strdup(input.toLocal8Bit().data());
}

void AddBorrowDialog::onClassSequenceFieldTextChanged(const QString &input) {
  borrowToAdd.classSequence = strdup(input.toLocal8Bit().data());
}

void AddBorrowDialog::on_confirmAdd_clicked() {
  // Add Borrow
  borrowToAdd.startTimestamp = time(NULL);
  borrowToAdd.borrowID = 0;
  borrowToAdd.status = ONGOING;
  borrowToAdd.bookID = 0;
  LibraryCreateBorrow(&borrowToAdd);

  // Update Table
  mainTableParent->showBorrowTable();

  free(borrowToAdd.name);
  free(borrowToAdd.classSequence);
  accept();
}


BookListModel::BookListModel(){
  rows = 0;
}

int BookListModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return rows;
}

QVariant BookListModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    Book book = data_[index.row()];
    return QString(book.title);
  }
  return QVariant();
}

// WHAT THE FUCK
int BookListModel::loadMore(Scrolling scrolling) {
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
  QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
  emit dataChanged(topLeft, bottomRight);

  return 0;
}

// WHAT THE FUCK
int BookListModel::searchData(Scrolling scrolling, const char *keyword) {
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
  QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
  emit dataChanged(topLeft, bottomRight);

  return 0;
}

void AddBorrowDialog::on_searchBox_activated(int index){
  if(index >=0){
    printf("3\n"); fflush(stdout);
    QModelIndex modelIndex = ui->searchBox->model()->index(index, 0);
    QString selectedText = ui->searchBox->model()->data(modelIndex, Qt::DisplayRole).toString();
    ui->searchBox->setEditText(selectedText);
  }
}

