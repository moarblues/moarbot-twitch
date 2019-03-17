#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    w.show();

    qsrand((uint)QDateTime::currentMSecsSinceEpoch()/1000);

    return a.exec();
}
