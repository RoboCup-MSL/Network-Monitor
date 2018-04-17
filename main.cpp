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
#include "robocop_funcs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QString teamB("./equipa2.txt");
    //QString teamA("./equipa1.txt");
    //QString teamC("./equipa3.txt");
    //QString capFile("./lab-03.csv");
    QString iw_name("wlp7s0");
    QString mon_name("mon0");
    stop_iw_mon(mon_name);
    start_iw_mon(iw_name);

    //get_team_by_file(teamA);

    get_all_teams();

    printAllPlayers();
    printAllTeams();
    qDebug("##############################################################\n");
    //parseNetCapture(capFile);
    qDebug("##############################################################\n");
    printAllTeams();
    printAllPlayers();





    MainWindow w;

    //set app name
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("RoboCop"));
    w.show();

    qDebug() << "end"<<endl;

    return a.exec();
    //return 0;
}
