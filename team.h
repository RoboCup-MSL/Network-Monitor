#ifndef TEAM_H
#define TEAM_H
#include "player.h"
#include <vector>
using namespace std;

#define TEAM_NONE "TEAM_NONE"

class team
{
    vector<player> players;
    bool inGame;
    QString tname;
public:
    //Constructor
    team(QString new_name);

    //Setters
    void insertPlayer(player &new_player);

    //Getters
    QString name();
    bool get_player(unsigned int num, player &r_player);
    bool get_player_by_mac(QString new_sta_mac, player &r_player);
    int get_team_size();
};

#endif // TEAM_H
