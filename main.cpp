#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>
#include <QInputDialog>
#include <qobject.h>
#include <vector>

#include "mainwindow.h"
#include "team.h"
#include "player.h"
#include "robo_log.h"
#include "robocop_funcs.h"
#include "iw.h"

iw wireless_obj;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    //QString teamB("./equipa2.txt");
//    //QString teamA("./equipa1.txt");
//    //QString teamC("./equipa3.txt");
//    //QString capFile("./lab-03.csv");
//    QString iw_name("wlp7s0");
//    QString mon_name("wlp7s0mon");
//    stop_iw_mon(mon_name);
//    start_iw_mon(iw_name);

//    //get_team_by_file(teamA);


//    printAllPlayers();
//    printAllTeams();
//    qDebug("##############################################################\n");
//    //parseNetCapture(capFile);
//    qDebug("##############################################################\n");
//    printAllTeams();
//    printAllPlayers();

    //stop all interface monitors
    open_robolog();
    QStringList *iw_list = wireless_obj.get_wl_interfaces();
    wireless_obj.stop_all_mon_interfaces(*iw_list);
    delete iw_list;

    get_all_teams();

    MainWindow w;

    //set app name
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("Roboc0p - Robocup's network monitor - BETA"));

    w.show();

    qDebug() << "end"<<endl;

    return a.exec();
/*    iw wireless_obj;
    QStringList *iw_list = wireless_obj.get_wl_interfaces();
    wireless_obj.stop_all_mon_interfaces(*iw_list);
    delete iw_list;
    iw_list=wireless_obj.get_wl_interfaces();
    wireless_obj.set_cap_wl_interface(iw_list->at(0));
    wireless_obj.get_wl_networks();
    wireless_obj.start_iw_monitor_mode();
    wireless_obj.set_cap_bssid(QString("38:D5:47:80:7F:E8"));
    wireless_obj.set_cap_channel(2);
    wireless_obj.start_capture();


    QThread::sleep(10);

    wireless_obj.stop_capture();

    QThread::sleep(10);

    return 0;
    */
}






















