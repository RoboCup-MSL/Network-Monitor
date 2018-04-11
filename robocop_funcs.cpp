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
    team* team1;

    if(team_descriptor.open(QIODevice::ReadOnly | QIODevice::Text)!= true){
        qDebug() << "Error, Couldn't open file" << team_file << endl;
        return false;
    }

    while(!tstream.atEnd()){
        QString line = tstream.readLine();
        if(line.at(0)=='#')
            continue;
        if(line.startsWith("Team Name:") == true){
            QStringList tname = line.split("Team Name:", QString::SkipEmptyParts);
            qDebug() << "Found Team Name: " <<  qPrintable((tname.at(0)).trimmed()) << endl;
            team1 = new team(tname.at(0));
            while(!tstream.atEnd()){
                QString line2 = tstream.readLine();
                if(line2.at(0)=='#')
                    continue;
                QStringList tplayer = line2.split(",", QString::SkipEmptyParts);
                if(tplayer.size() < 2){
                    qDebug() << "Invalid Player: " << qPrintable(line2) << endl;
                    continue;
                }

                qDebug() << "Found Candidate for Player: " <<  qPrintable((tplayer.at(1)).trimmed()) << " with MAC: "
                           <<  qPrintable((tplayer.at(0)).trimmed()) << endl;

                player* player1 = new player(tplayer.at(1), tplayer.at(0), tname.at(0));
                AllPlayers.push_back(*player1);
                team1->insertPlayer(*player1);
            }
        }
    }
    AllTeams.push_back(*team1);
    return true;
}

void printAllPlayers(void){

    qDebug() << "###############ALL PLAYERS LIST #################" << endl;

    for(unsigned int i = 0; i < AllPlayers.size(); i++){
        qDebug("Player %3d, Name %20s, MAC %20s, Team %10s\n", i, qPrintable(AllPlayers[i].name()), qPrintable(AllPlayers[i].mac()), qPrintable(AllPlayers[i].team_name()));
    }

}

void printAllTeams(void){

    qDebug() << "###############ALL TEAMS LIST #################" << endl;

    for(unsigned int i = 0; i<AllTeams.size(); i++){
       qDebug("**TEAM %s\n", qPrintable(AllTeams[0].name()));
       for(int j = 0; j < AllTeams[i].get_team_size(); j++){
            player playerx;
            AllTeams[i].get_player(j, playerx);
            qDebug("Player %3d, Name %20s, MAC %20s, Team %10s\n", j, qPrintable(playerx.name()), qPrintable(playerx.mac()), qPrintable(playerx.team_name()));

       }
             
       
    }



}
