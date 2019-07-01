#ifndef IW_H
#define IW_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <syslog.h>

class iw
{
public:
    /**
     * @brief Constructor for Wireless Interface management class
     */
    iw();

    /**
     * @brief Fetch all Wireless 802.11 Interfaces on the host
     * @return  List of names of the Wireless interfaces on the host
     */
    QStringList *get_wl_interfaces(void);

    /**
     * @brief Fetch Information about all detected wireless networks
     * @return returns a list of of the wireless networks and their information in comma separated values
     * format: <SSID,BSSID,CHANNEL>\n If network is hidden SSID will appear as "-"
     */
    QStringList *get_wl_networks(void);


    /**
     * @brief Set Wireless Capture filename prefix name/path for Airodump data dump
     * @param prefix prefix of the data dump file
     */
    void set_cap_file_prefix(QString prefix);

    /**
     * @brief Set/Choose WL interface to monitor/analyze
     * @param cap_interface name of the interface to be monitored
     */
    void set_cap_wl_interface(QString cap_interface);

    /**
     * @brief Set/Choose Network to be monitored by passing the BSSID to the IW object
     * @param bssid BSSID name of the network to be analyzed/monitored/policed
     */
    void set_cap_bssid(QString bssid);


    /**
     * @brief Set/Choose Wireless Channel of where the network to monitor is radiating
     * @param channel channel number
     */
    void set_cap_channel(uint channel);

    /**
     * @brief Put the selected network interface this function must be only called after correct
     * set of wireless interface to monitor by \ref set_cap_wl_interface
     * @return
     */
    bool start_iw_monitor_mode();
    /**
     * @brief Stop Monitoring mode for all interfaces provided
     * @param iw_list List of names of Wireless interfaces to which the object should try stop Monitor mode
     * \n Usually this param is obtained by \ref get_wl_interfaces
     */
    void stop_all_mon_interfaces(QStringList iw_list);

    /**
     * @brief Start Capturing Packets and data from the wireless network chosen and dump it to the file
     * \n A capture dump file is automatically created everytime a capture is started
     * @return  TRUE if capture process is launched sucessfuly, FALSE otherwise
     */
    bool start_capture();

    /**
     * @brief Stop a capture previously running
     * @return TRUE if capture process is stopped sucessfuly, FALSE otherwise
     */
    bool stop_capture();

private:
    uint cap_channel;
    QString cap_file_prefix;
    QString cap_wl_interface;
    QString cap_bssid;
    QProcess* cap_airodump;
    QProcess* wl_helper;
};

#endif // IW_H
