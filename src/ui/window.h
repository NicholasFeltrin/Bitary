#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "mainTableModel.h"
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

    void on_loadMore_clicked();
    void on_loadMore_pressed();

private:
    Ui::MainWindow *ui;
    mainTableModel *bookModel;
};


#endif // WINDOW_H
