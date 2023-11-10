#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainTable.h"
#include "../forms/ui_addBookPopUpWindow.h"
#include "../forms/ui_addBorrowPopUpWindow.h"
extern "C"{
#include "src/library/library.h"
}

class AddBookDialog;
class AddBookDialog;
class BookListModel;

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
    explicit AddBorrowDialog(MainTable *parent = nullptr);
    ~AddBorrowDialog();

  private slots:
    void on_searchBox_activated(int index);
    void onNameFieldTextChanged(const QString &input);
    void onClassSequenceFieldTextChanged(const QString &input);
    void on_confirmAdd_clicked();

  private:
    Ui::AddBorrowDialog *ui;

    Borrow borrowToAdd;
    BookListModel *bookListModel;

    MainTable *mainTableParent;
};

class BookListModel : public QAbstractListModel {
  Q_OBJECT

  public:
    BookListModel(void);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int loadMore(Scrolling scrolling);
    int searchData(Scrolling scrolling, const char *keyword);

  private:
    int rows;
    Book *data_;
};

#endif // DIALOG_H
