#ifndef ROBOCOP_FUNCS_H
#define ROBOCOP_FUNCS_H

#include <QString>
#include <vector>

bool get_team_by_file(QString team_file);

void get_all_teams(void);

void printAllPlayers(void);

void printAllTeams(void);

void parseNetCapture(QString capture_file);

team *get_team_by_name(QString tname);

void print_team(team *a_team);

bool start_iw_mon(QString iw);

bool stop_iw_mon(QString iw);

void clean_AllPlayers_stat(void);

extern vector<player *>   AllPlayers;
extern vector<team>     AllTeams;
extern QString      team_none_name;

#endif // ROBOCOP_FUNCS_H
