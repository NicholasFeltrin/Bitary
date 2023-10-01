#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "mainTable.h"
extern "C" {
#include "src/library/library.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private slots:
  void on_testcreate_clicked();

  void on_Books_clicked();
  void on_Borrows_clicked();

private:
  Ui::MainWindow *ui;

  MainTable *mainTable; 
};


#endif // WINDOW_H
