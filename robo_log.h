#ifndef ROBO_LOG_H
#define ROBO_LOG_H

#include <syslog.h>

#define APP_NAME "RoboC0p"

#define ROBOLOG_INFO(format, ...)           syslog(LOG_INFO, format, ## __VA_ARGS__)
#define ROBOLOG_NOTICE(format, ...)         syslog(LOG_NOTICE, format, ## __VA_ARGS__)
#define ROBOLOG_ALERT(format, ...)          syslog(LOG_ALERT, format, ## __VA_ARGS__)

#define BW_ALERT            "BANDWIDTH ALARM"
#define BW_INFO             "BANDWIDTH INFO"
#define BW_NOTICE           "BANDWIDTH NOTICE"
#define STATION_INFO        "STATION INFO"
#define STATION_ALERT       "STATION ALARM"
#define GAME_NOTICE         "GAME NOTICE"
#define GAME_START          "GAME START"
#define GAME_END            "GAME END"
#define TEAM_INFO           "TEAM INFO"


inline void open_robolog(void){
openlog(APP_NAME, LOG_CONS | LOG_PID, LOG_USER);
syslog(LOG_INFO,"HELLO FROM %s - APP is RUNNING", APP_NAME);
}

inline void close_robolog(void){
    syslog(LOG_INFO,"HELLO FROM %s - APP is Closing Log", APP_NAME);
    closelog();
}






#endif // ROBO_LOG_H
