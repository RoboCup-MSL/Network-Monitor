#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <QDateTime>
#include <QString>


class player
{
private:
    QString sta_name;
    QString sta_mac;
    QDateTime sta_first_time_seen;
    QDateTime sta_last_time_seen;
    int sta_power;
    int sta_packets;
    QString sta_team;
    bool sta_connected;
    int sta_bandwith;
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
    void update(QDateTime first_seen, QDateTime last_seen, int packets, int power);

    //Getters
    QString name();
    QString mac();
    QDateTime firstTimeSeen();
    QDateTime lastTimeSeen();
    bool isConnected();


};

#endif // PLAYER_H
