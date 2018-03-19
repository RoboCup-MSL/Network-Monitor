#include "player.h"

player::player()
{
    sta_name = QString("");
    sta_first_time_seen = QDateTime();
    sta_last_time_seen = QDateTime();
    sta_power=0;
    sta_packets=0;
    sta_team = QString("");
    sta_connected = false;
    sta_bandwith = 0;
}

player::player(QString name, QString mac, QString team)
{
    sta_name = name;
    sta_mac = mac;
    sta_team = team;
}

player::player(QString mac)
{
    sta_mac = mac;
}

void player::update(QDateTime first_seen, QDateTime last_seen, int packets, int power)
{
    sta_first_time_seen = first_seen;
    sta_last_time_seen = last_seen;
    sta_packets = packets;
    sta_power = power;

}

QString player::name()
{
    return sta_name;
}

QDateTime player::firstTimeSeen()
{
    return sta_first_time_seen;
}

QDateTime player::lastTimeSeen(){
    return sta_last_time_seen;
}

QString player::mac(){
    return sta_mac;
}

void player::connected()
{
    sta_connected = true;
}

void player::disconnected()
{
    sta_connected = false;
}

bool player::isConnected()
{
    return sta_connected;
}




