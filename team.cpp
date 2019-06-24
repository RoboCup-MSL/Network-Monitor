#include "team.h"
#include <QDebug>


team::team(QString new_name){
    tname = QString(new_name.trimmed());
    inGame = false;
    bandwith = 0;
    bandwith_alarm = false;
}

void team::set_inGame(){
    inGame = true;
    bandwith = 0;
    bandwith_alarm = 0;
}

bool team::is_inGame(){
    return inGame;
}

void team::clean_stats(){
    inGame = false;
    bandwith = 0;
    bandwith_alarm = 0;
}



void team::insertPlayer(player *new_player){
    players.push_back(new_player);
}

bool team::get_player(unsigned int num, player **r_player){
    if(players.size() < 1 ){
        qDebug() << "There are no players on the team " << tname << endl;
        return false;
    }

    if( (num >= players.size()) ){
        qDebug() << "Player number " << num <<" doesnt exist on team" << tname <<endl;
        return false;
    }

    *r_player = players[num];
    return true;
}

int team::get_team_size(){
    return players.size();
}

bool team::get_player_by_mac(QString new_sta_mac, player **r_player){
    for(unsigned int i = 0 ; i < players.size(); i++){
        if(players[i]->mac() == new_sta_mac){
            *r_player = players[i];
            return true;
        }
    }
    return false;
}

void team::updateTeam(){
    bandwith = 0; // reset bandwith
    for(uint i = 0; i < players.size(); i++){
        bandwith += players[i]->throughput();
    }
    if (!bandwith_alarm)
        bandwith_alarm = ((bandwith*8)>=BW_MAX);
    else
        bandwith_alarm = !((bandwith*8)<(BW_MAX - BW_HYSTERESIS));
    qDebug("Team %s is using %u bit/s allarmed %s\n", qPrintable(tname), bandwith*8, bandwith_alarm? "yes":"no");

}

uint team::throughput(){
    return bandwith;
}

QString team::name(){
    return tname;
}

bool team::bwAlarmed(){
    return bandwith_alarm;
}
