#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>

#include <qobject.h>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "start"<<endl;
    MainWindow w;
    w.show();

    qDebug() << "end"<<endl;

    return a.exec();
}
