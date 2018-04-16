#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <QProcess>
#include "player.h"
#include "team.h"


enum STATION_PROPERTIES
{
    STATION_MAC = 0,
    FIRST_TIME_SEEN = 1,
    LAST_TIME_SEEN = 2,
    POWER = 3,
    PACKETS = 4,
};

enum PLAYER_PROPERTIES{
    PLAYER_MAC = 0,
    PLAYER_NAME = 1,
};

#define MIN_PROPERTIES 5

vector<player *>   AllPlayers;
vector<team>     AllTeams;
static player* findStation(QString sta_mac);
static team* get_team_by_name(QString tname);
QString team_none_name(TEAM_NONE);


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
            if(get_team_by_name(tname.at(0)) != NULL){
                qDebug("Team %s Already Exists in the system, will skip", qPrintable(tname.at(0).trimmed()));
                return false;
            }
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
                player *playerx;
                if((playerx = findStation(tplayer.at(PLAYER_MAC).trimmed()))==NULL){
                    qDebug("STA_MAC %s not already in the player LIST will INSERT\n",qPrintable(tplayer.at(PLAYER_MAC).toUpper().trimmed()) );
                    player* player1 = new player(tplayer.at(PLAYER_NAME).trimmed(), tplayer.at(PLAYER_MAC).trimmed(), tname.at(0).trimmed());
                    AllPlayers.push_back(player1);
                    team1->insertPlayer(player1);

                }
                else{
                    qDebug("MAC %s already exists in data base in team %s, will skip",qPrintable(tplayer.at(PLAYER_MAC).trimmed()), qPrintable(playerx->team_name()));
                    continue;
                }

            }
        }
    }
    AllTeams.push_back(*team1);
    delete team1;
    return true;
}

void printAllPlayers(void){

    qDebug() << "###############ALL PLAYERS LIST #################" << endl;

    for(unsigned int i = 0; i < AllPlayers.size(); i++){
        qDebug("Player %3d, Name %20s, MAC %20s, Team %10s, First Time Seen %s, Last Time Seen %s, Power %d dBm, Packets %d\n",
               i, qPrintable(AllPlayers[i]->name()), qPrintable(AllPlayers[i]->mac()),
               qPrintable(AllPlayers[i]->team_name()),
               qPrintable(AllPlayers[i]->firstTimeSeen().toString("dd-MM-yyyy hh:mm:ss")),
               qPrintable(AllPlayers[i]->lastTimeSeen().toString("dd-MM-yyyy hh:mm:ss")), AllPlayers[i]->power(), AllPlayers[i]->packets());
    }

}

void printAllTeams(void){

    qDebug() << "###############ALL TEAMS LIST #################" << endl;

    for(unsigned int i = 0; i<AllTeams.size(); i++){
       qDebug("**TEAM %s, size %d\n", qPrintable(AllTeams[i].name()), (int)AllTeams[i].get_team_size());
       for(int j = 0; j < AllTeams[i].get_team_size(); j++){
            player *playerx;
            AllTeams[i].get_player(j, &playerx);
            qDebug("Player %3d, Name %20s, MAC %20s, Team %10s\n", j, qPrintable(playerx->name()), qPrintable(playerx->mac()), qPrintable(playerx->team_name()));
       }
      }
}

void parseNetCapture(QString capture_file){

    QFile capture_descriptor(capture_file);
    QTextStream cap_stream(&capture_descriptor);

    if(capture_descriptor.open(QIODevice::ReadOnly | QIODevice::Text)!= true){
        qDebug() << "Error, Couldn't open file" << qPrintable(capture_file) << endl;
        return;
    }

    while(!cap_stream.atEnd()){
        QString line = cap_stream.readLine();
        //qDebug("%s\n",qPrintable(line));
       if(line.startsWith("Station MAC")==false)
           continue;
       while(!cap_stream.atEnd()){
           QString line2 = cap_stream.readLine();
           QStringList sta_props = line2.split(",", QString::SkipEmptyParts);
           if(sta_props.size()<MIN_PROPERTIES){
               qDebug("Invalid STATION %s\n",qPrintable(line2));
               continue;
           }
           QString sta_mac = sta_props.at(STATION_MAC).trimmed();

           QDateTime first_seen = QDateTime::fromString(sta_props.at(FIRST_TIME_SEEN).trimmed(),"yyyy-MM-dd hh:mm:ss");
           QDateTime last_seen = QDateTime::fromString(sta_props.at(LAST_TIME_SEEN).trimmed(),"yyyy-MM-dd hh:mm:ss");
           int power = sta_props.at(POWER).toInt();
           int packets = sta_props.at(PACKETS).toInt();
           qDebug("##Candidate Station Found \n");
           qDebug("STA_MAC %s, First Time Seen %s, Last Time Seen %s, Power %d dBm, Packets %d \n",
                  qPrintable(sta_mac), qPrintable(first_seen.toString("dd-MM-yyyy hh:mm:ss")), qPrintable(last_seen.toString("dd-MM-yyyy hh:mm:ss")), power, packets);

           player *player1;

           if((player1 = findStation(sta_mac) ) != NULL){
               qDebug("Station is already at database with %s, %s, %s", qPrintable((player1->mac())), qPrintable((player1->name())), qPrintable(player1->team_name()));
               player1->update(first_seen, last_seen, packets, power);
           }else{
               qDebug("New Station Detected in Capture FILE %s, will insert in database and update stats \n", qPrintable(sta_mac));
               player *sta_new = new player(sta_mac, team_none_name);
               sta_new->update(first_seen, last_seen, packets, power);
               AllPlayers.push_back(sta_new);
               // Evaluate if Team None already exists
               team *team_none;
                        if((team_none = get_team_by_name(team_none_name)) != NULL){
                   qDebug("Team %s Already Exists in the system, insert player", qPrintable(team_none->name()));
                   team_none->insertPlayer(sta_new);
                 }else{
                   team_none = new team(team_none_name);
                   team_none->insertPlayer(sta_new);
                   AllTeams.push_back(*team_none);
                   delete team_none;
                 }

           }



       }

    }
}



static player* findStation(QString sta_mac){

    for(unsigned int i = 0; i < AllPlayers.size(); i++){
        if(QString::compare(sta_mac, AllPlayers[i]->mac(), Qt::CaseInsensitive) == 0)
            return (AllPlayers[i]);
    }

    return NULL;


}

static team *get_team_by_name(QString tname){

    for(unsigned int i = 0; i < AllTeams.size(); i++){
        if(tname.trimmed() == AllTeams[i].name())
                return &AllTeams[i];
    }

    return NULL;


}


bool start_iw_mon(QString iw){
    QString command;
    command = QString::asprintf("sudo airmon-ng start %s", qPrintable(iw));

    return (QProcess::execute(command)==QProcess::NormalExit);

}

bool stop_iw_mon(QString iw){
    QString command;
    command = QString::asprintf("sudo airmon-ng stop %s", qPrintable(iw));
    return (QProcess::execute(command)==QProcess::NormalExit);
}




