#include "mainwindow.h"
#include <QMainWindow>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //create the gui
    robocopGui = new gui(this);
    setCentralWidget(robocopGui);
}
