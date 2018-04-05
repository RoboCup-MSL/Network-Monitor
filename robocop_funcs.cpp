#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "player.h"
#include "team.h"

extern vector<player>   AllPlayers;
extern vector<team>     AllTeams;


bool get_team_by_file(QString team_file){

    QFile team_descriptor(team_file);
    QTextStream tstream(&team_descriptor);

    if(team_descriptor.open(QIODevice::ReadOnly | QIODevice::Text)!= true){
        qDebug() << "Error, Couldn't open file" << team_file << endl;
        return false;
    }

    while(!tstream.atEnd()){
        QString line = tstream.readLine();
        if(line.at(0)=='#')
            continue;
        if(line.startsWith("Team Name:") == true){
            QStringList tname = line.split("Team Name:",QString::SkipEmptyParts);
            qDebug() << "Found Team Name: " << (tname.at(0)).trimmed() << endl;
            //while(!tstream.atEnd()){
        }
    }


    return true;



}
