#include "iw.h"

iw::iw()
{
 cap_file_prefix = "default";
}
void iw::set_cap_wl_interface(QString iface){
    cap_wl_interface = iface;
}

void iw::set_cap_file_prefix(QString prefix){
    cap_file_prefix = prefix;
}

void iw::set_cap_bssid(QString bssid){
    cap_bssid = bssid;
}

void iw::set_cap_channel(uint channel){
    cap_channel = channel;
}




QStringList* iw::get_wl_interfaces(void){
    wl_helper = new QProcess();
    wl_helper->start("sudo airmon-ng");
    wl_helper->waitForFinished(-1); // will wait forever until finished

    QString wl_output = (wl_helper->readAllStandardOutput()).trimmed();
    QString wl_err = wl_helper->readAllStandardError();

    qDebug("wl_interfaces:\n%s", qPrintable(wl_output));
    QStringList *iList = new QStringList();

    QStringList iw_list = wl_output.split("\n",QString::SkipEmptyParts);
    for(int i = 0; i < iw_list.size(); i++){
       qDebug(qPrintable(iw_list.at(i)));
       if(i==0)
           continue; // first line doesn't have information
       QStringList auxiliar = iw_list.at(i).split("\t",QString::SkipEmptyParts);
       if(auxiliar.size()>1) // validate info
            iList->append(auxiliar.at(1));
    }
    qDebug("\niList:");
    for(int i = 0; i < iList->size(); i++){
        qDebug("%s",qPrintable(iList->at(i)));
    }
    qDebug("\nwl_error:\n%s", qPrintable(wl_err));
    delete wl_helper;

    return iList;
}

QStringList* iw::get_wl_networks(){

    QStringList *netList = new QStringList();

    if(cap_wl_interface.isEmpty()){
        qDebug("Error: There is No Capture interface selected, cannot scan for networks\n");
        return netList;
    }

    /* WL SCAN */
    qDebug("\nScanning Networks\n");
    wl_helper = new QProcess();
    QString command = QString::asprintf("iwlist %s scan", qPrintable(cap_wl_interface));
    wl_helper->start(command);
    wl_helper->waitForFinished(-1); // will wait forever until finished
    /* WL SCAN */

    /* Fetch networks in beautiful way */


    wl_helper->start("nmcli -f SSID,BSSID,CHAN,ACTIVE dev wifi list");
    wl_helper->waitForFinished(-1); // will wait forever until finished

    QString wl_output = wl_helper->readAllStandardOutput();
    QString wl_err = wl_helper->readAllStandardError();

    qDebug("\nwl_ouptut:\n%s", qPrintable(wl_output));
    qDebug("\nwl_err:\n%s", qPrintable(wl_err));



    QStringList wl_list = wl_output.split("\n",QString::SkipEmptyParts);
    for(int i = 0; i < wl_list.size(); i++){
       qDebug(qPrintable(wl_list.at(i)));
       if(i==0)
           continue; // first line doesn't have information
       QStringList auxiliar = wl_list.at(i).split(" ",QString::SkipEmptyParts);
       if(auxiliar.size()>=3) // validate info
           netList->append(auxiliar.at(0) + "," + auxiliar.at(1) + "," + auxiliar.at(2));
    }
    qDebug("\nnetList:");
    for(int i = 0; i < netList->size(); i++){
        qDebug("%s",qPrintable(netList->at(i)));
    }
    qDebug("\nwl_error:\n%s", qPrintable(wl_err));
    delete wl_helper;

    return netList;

}

void iw::stop_all_mon_interfaces(QStringList iw_list){

    if(iw_list.size()<1)
        return;
    for(int i=0; i< iw_list.size(); i++){
        if(iw_list.at(i).contains("mon", Qt::CaseInsensitive)){
            wl_helper = new QProcess();
            QString command = QString::asprintf("airmon-ng stop %s",qPrintable(iw_list.at(i)));
            qDebug("Stopping mon interface %s", qPrintable(iw_list.at(i)));
            wl_helper->start(command);
            wl_helper->waitForFinished(10E3); // Wait for process finish
            delete wl_helper;
        }
    }

}
bool iw::start_iw_monitor_mode(){
    bool ret;
    if(cap_wl_interface.size()>0){
        wl_helper = new QProcess();
        QString command = QString::asprintf("airmon-ng start %s",qPrintable(cap_wl_interface));
        qDebug("Starting mon interface on %s", qPrintable(cap_wl_interface));
        wl_helper->start(command);
        wl_helper->waitForFinished(10E3);
        qDebug("\nairmon exit code %d\n",wl_helper->exitCode());
        QString wl_output = wl_helper->readAllStandardOutput();
        QString wl_err = wl_helper->readAllStandardError();
        qDebug("\nwl_ouptut:\n%s", qPrintable(wl_output));
        qDebug("\nwl_err:\n%s", qPrintable(wl_err));
        ret = wl_helper->exitCode() == QProcess::NormalExit;
        delete wl_helper;
        return ret;
    }
    else{
        qDebug("There is no interface defined/selected\n");
        return false;
    }

}

bool iw::start_capture(){
    cap_airodump = new QProcess();
    qDebug("\n STARTING CAPTURE \n");
    QString command =  QString::asprintf("sudo airodump-ng -c %d -M -d %s -w %s --write-interval 1 --output-format csv %smon",
                                       cap_channel, qPrintable(cap_bssid), qPrintable(cap_file_prefix), qPrintable(cap_wl_interface));

    qDebug("%s", qPrintable(command));
    cap_airodump->start(command);

    if(cap_airodump->waitForStarted()!=true)
    {
        qDebug("Couldn't start airodump");
        return false;
    }

    return true;

}

bool iw::stop_capture(){
    qDebug("\n STOPPING CAPTURE \n");
    if(cap_airodump->state()!=QProcess::NotRunning)
    {
        cap_airodump->kill();
        if(cap_airodump->waitForFinished(1000)!=true)
        {
            qDebug("Couldn't kill airodump");
            return false;
        }
    }
    delete cap_airodump;
    return true;
}


