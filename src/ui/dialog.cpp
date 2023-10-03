#include <QMainWindow>
#include <QLineEdit>
#include <qdialog.h>
#include "dialog.h"
#include "../forms/ui_addBookPopUpWindow.h"
#include "../forms/ui_borrowBookPopUpWindow.h"
#include "src/library/library.h"

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


AddBorrowDialog::AddBorrowDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddBorrowDialog) {
  ui->setupUi(this);
}

AddBorrowDialog::~AddBorrowDialog() {
  delete ui;
}




