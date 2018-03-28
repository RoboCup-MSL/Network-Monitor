#include "team.h"
#include <QDebug>


team::team(QString new_name){
    name = new_name;
    inGame = false;
}

void team::insertPlayer(player new_player){
    players.push_back(new_player);
}

bool team::get_player(unsigned int num, player &r_player){
    if(players.size() < 1 ){
        qDebug() << "There are no players on the team " << name << endl;
        return false;
    }

    if( (num >= players.size()) ){
        qDebug() << "Player number " << num <<" doesnt exist on team" << name <<endl;
        return false;
    }

    r_player = players[num];
    return true;
}

int team::get_team_size(){
    return players.size();
}

bool team::get_player_by_mac(QString new_sta_mac, player &r_player){
    for(unsigned int i = 0 ; i < players.size(); i++){
        if(players[i].mac() == new_sta_mac){
            r_player = players[i];
            return true;
        }
    }
    return false;
}
