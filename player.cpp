#include "player.h"
#include <limits>

player::player(){
}

player::player(QString name, QString mac, QString team)
{
    sta_name = QString(name);
    sta_mac = QString(mac.toUpper()); // Saving MAC addresses always in the same case
    sta_team = QString(team);
    sta_power = std::numeric_limits<int>::min();
    sta_packets = 0;
    sta_connected = false;
    sta_bandwith = 0;
    sta_pkts_sec = 0;
}

player::player(QString mac, QString team)
{   sta_team = QString(team);
    sta_mac = QString(mac.toUpper()); // Saving MAC addresses always in the same case
    sta_power = std::numeric_limits<int>::min();
    sta_power = std::numeric_limits<int>::min();
    sta_packets = 0;
    sta_connected = false;
    sta_bandwith = 0;
    sta_pkts_sec = 0;

}

void player::update(QDateTime first_seen, QDateTime last_seen, int packets, int power)
{
    // Calculate packets per second rough aproximation

    if((sta_last_time_seen.isValid() == true) && (last_seen.isValid() == true))
    {
        int diff_packets =  packets - sta_packets;
        qint64 diff_time_msec = sta_last_time_seen.msecsTo(last_seen);
        if(diff_time_msec >=0)
        sta_pkts_sec = diff_packets*1000/diff_time_msec;
    }else
        if((first_seen.isValid() == true) && (last_seen.isValid() == true))
        {
           qint64 diff_time_msec = first_seen.msecsTo(last_seen);
           if(diff_time_msec >=0)
           sta_pkts_sec = packets*1000/diff_time_msec;
        }else{
            sta_pkts_sec = 0;
        }

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

void player::clean_stats(){

    sta_first_time_seen = QDateTime();
    sta_last_time_seen = QDateTime();
    sta_power = std::numeric_limits<int>::min();
    sta_packets = 0;
    sta_connected = false;
    sta_bandwith = 0;
}

uint player::pkts_second(){
    return sta_pkts_sec;
}

