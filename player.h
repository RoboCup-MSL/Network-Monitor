#ifndef PLAYER_H
#define PLAYER_H
#include <QDateTime>
#include <QString>
#define CONNECTION_TIMEOUT 30 // 30 sec for evaluating if station is connected

class player
{
private:
    QString sta_name;
    QString sta_mac;
    QDateTime sta_first_time_seen;
    QDateTime sta_last_time_seen;
    int sta_power = 0;
    uint sta_packets = 0;
    QString sta_team;
    bool sta_connected;
    uint sta_bandwith;  // bytes per second
    uint sta_pkts_sec; // packets per second
    uint sta_bytes;
public:
    //Constructors
    player();
    player(QString name, QString mac, QString team);
    player(QString mac);
    //End Constructors

    //Acessors
    //Setters
    void disconnected();
    void connected();
    void update(QDateTime first_seen, QDateTime last_seen, uint packets, int power, uint bytes);
    void clean_stats();

    //Getters
    QString name();
    QString mac();
    QString team_name();
    QDateTime firstTimeSeen();
    QDateTime lastTimeSeen();
    uint pkts_second();
    bool isConnected();
    int power();
    uint packets();
    uint throughput();
};

#endif // PLAYER_H
