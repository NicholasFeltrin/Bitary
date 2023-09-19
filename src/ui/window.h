#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <qabstractitemmodel.h>
#include <qtableview.h>
#include "models.h"
extern "C" {
#include "src/library/library.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private slots:
  void on_loadMore_clicked();

  void on_testcreate_clicked();


  void on_Books_clicked();
  void on_Borrows_clicked();

private:
  Ui::MainWindow *ui;

  void initMainTable();

  QTableView *mainTableView;

  BookModel *bookModel;
  BorrowModel *borrowModel;
};


class MainTableView : public QTableView {
  Q_OBJECT

public:
  MainTableView(QWidget *parent = nullptr);

protected:
 void scrollContentsBy(int dx, int dy) override
    {
        QTableView::scrollContentsBy(dx, dy);

        // Check if the user is scrolling near the end of the rows
        if (dy > 0 && verticalScrollBar()->value() >= verticalScrollBar()->maximum() * 0.8)
        {
            // Determine which model is currently active and call the loadmore() method on it
            if (model() == &model1)
            {
                model1.loadmore();
            }
            else if (model() == &model2)
            {
                model2.loadmore();
            }
            // Add more conditions for other models as needed
        }
    }


};


#endif // WINDOW_H
