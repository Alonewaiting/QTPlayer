#include "FFPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FFPlayer window;
    window.show();
    return app.exec();
}
