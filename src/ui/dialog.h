#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainTable.h"
#include "../forms/ui_addBookPopUpWindow.h"
#include "../forms/ui_borrowBookPopUpWindow.h"
extern "C"{
#include "src/library/library.h"
}


class AddBookDialog : public QDialog {
  Q_OBJECT

  public:
    explicit AddBookDialog(MainTable *parent = nullptr);
    ~AddBookDialog();

  public slots:
    void onTitleFieldTextChanged(const QString &input);
    void onLibraryIDFieldChanged(const QString &input);
    void on_confirmAdd_clicked();

  private:
    Ui::AddBookDialog *ui;

    Book bookToAdd;

    MainTable *mainTableParent;
};

class AddBorrowDialog : public QDialog {
  Q_OBJECT

  public:
    explicit AddBorrowDialog(QWidget *parent = nullptr);
    ~AddBorrowDialog();

  private:
    Ui::AddBorrowDialog *ui;

    MainTable *mainTableParent;
};

#endif // DIALOG_H
