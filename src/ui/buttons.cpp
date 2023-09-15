#include <QMainWindow>
#include "window.h"

void MainWindow::on_loadMore_clicked()
{
  loadBookDataChunk(&dataBookBuffer, LOADBEGINNING);
  bookModel->loadBooks(&dataBookBuffer);
}

