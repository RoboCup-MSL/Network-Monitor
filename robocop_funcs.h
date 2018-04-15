#ifndef ROBOCOP_FUNCS_H
#define ROBOCOP_FUNCS_H

#include <QString>
#include <vector>

bool get_team_by_file(QString team_file);

void printAllPlayers(void);

void printAllTeams(void);

void parseNetCapture(QString capture_file);

bool start_iw_mon(QString iw);

bool stop_iw_mon(QString iw);

extern vector<player>   AllPlayers;
extern vector<team>     AllTeams;


#endif // ROBOCOP_FUNCS_H
