#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>

#include <qobject.h>

#include "mainwindow.h"
#include "team.h"
#include "player.h"
#include <vector>

vector<player> AllPlayers;
vector<team>     AllTeams;

bool get_team_by_file(QString team_file);
void printAllPlayers(void);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "start"<<endl;
    MainWindow w;

    QString teamA("./equipa1.txt");
    QString teamB("./equipa2.txt");


    get_team_by_file(teamA);
    get_team_by_file(teamB);


    printAllPlayers();


    //w.show();

    qDebug() << "end"<<endl;

    //return a.exec();
    return 0;
}
