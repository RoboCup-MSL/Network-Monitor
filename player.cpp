#include "player.h"

player::player(){
}

player::player(QString name, QString mac, QString team)
{
    sta_name = QString(name);
    sta_mac = QString(mac.toUpper()); // Saving MAC addresses always in the same case
    sta_team = QString(team);
}

player::player(QString mac, QString team)
{   sta_team = QString(team);
    sta_mac = QString(mac.toUpper()); // Saving MAC addresses always in the same case
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

QString player::team_name(){
    return sta_team;
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

int player::power()
{
    return sta_power;
}

int player::packets()
{
    return sta_packets;
}




