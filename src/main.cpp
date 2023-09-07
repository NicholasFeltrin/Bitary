#include "ui/window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window W;
    W.show();
    return app.exec();
}
