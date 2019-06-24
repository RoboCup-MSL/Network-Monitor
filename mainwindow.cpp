#include "mainwindow.h"
#include <QMainWindow>
#include "iw.h"
#include "robo_log.h"

extern iw wireless_obj;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //create the gui
    robocopGui = new gui(this);
    setCentralWidget(robocopGui);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QStringList *iw_list = wireless_obj.get_wl_interfaces();
    wireless_obj.stop_all_mon_interfaces(*iw_list);
    delete iw_list;
    event->accept();
    close_robolog();
}
