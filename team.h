#ifndef TEAM_H
#define TEAM_H
#include "player.h"
#include <vector>
using namespace std;

#define BW_MAX 2200E3
#define BW_HYSTERESIS 200E3

class team
{
    vector<player *> players;
    bool inGame;
    QString tname;
    uint bandwith; //Used Bandwidth by all team members
    bool bandwith_alarm; // TEAM IS IN BW ALARM
public:
    //Constructor
    team(QString new_name);

    //Setters
    void insertPlayer(player *new_player);
    void updateTeam();
    void set_inGame();
    void clean_stats();

    //Getters
    QString name();
    bool get_player(unsigned int num, player **r_player);
    bool get_player_by_mac(QString new_sta_mac, player **r_player);
    int get_team_size();
    bool is_inGame();
    uint throughput();
    bool bwAlarmed();
};

#endif // TEAM_H
