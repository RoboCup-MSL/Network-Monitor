#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>
#include <QInputDialog>

#include <qobject.h>

#include "mainwindow.h"
#include "team.h"
#include "player.h"
#include <vector>

vector<player> AllPlayers;
vector<team>     AllTeams;

bool get_team_by_file(QString team_file);
void printAllPlayers(void);
void printAllTeams(void);
void parseNetCapture(QString capture_file);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //set app name
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("RoboCop"));
    QString teamB("./equipa2.txt");
    QString teamA("./equipa1.txt");
    QString teamC("./equipa3.txt");
    QString capFile("./lab-03.csv");
    //get_team_by_file(teamA);

    get_team_by_file(teamA);
    get_team_by_file(teamB);


    printAllPlayers();
    printAllTeams();
    qDebug("##############################################################\n");
    parseNetCapture(capFile);
    qDebug("##############################################################\n");
    printAllPlayers();






    //w.show();

    qDebug() << "end"<<endl;

    //return a.exec();
    return 0;
}
