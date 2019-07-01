/** @file gui.c
 *  @brief GUI implementation
 *
 *  This contains the UI functions
 *
 *  @author Ivo Angélico
 *  @author César Gomes
 */
/* -- Includes -- */
/* QT libs includes. */

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QBoxLayout>
#include <QApplication>
#include <QTableWidget>

/* Roboc0p objects includes. */
#include "gui.h"
#include "team.h"
#include "robocop_funcs.h"
#include "iw.h"

/* external variables */
extern vector<team>     AllTeams;
extern iw wireless_obj;

/** @brief gui constructor
 *
 *  Initializes the GUI and other necessary objects
 *
 *  @param QWidget* parent widger
 */
gui::gui(QWidget *parent) : QWidget(parent)
{
    //if there aren't teams we're here doing nothing
    if(AllTeams.size()==0)
    {
        QMessageBox::warning(this,
            tr("Error!"),
            tr("There are no teams!") );
        return;
    }

    //initialize airodump
    airodump = new QProcess(this);

    //this will be the base layout
    QHBoxLayout *baseLayout;
    baseLayout = new QHBoxLayout(this);
    toolbox = new QToolBox;
    toolbox->setMinimumSize(750, 500);
    baseLayout->addWidget(toolbox);
    //use grid
    inputsPage = new QWidget;
    inputsLayout = new QGridLayout(inputsPage);

    //Interfaces and network droplists
    //create button to set interface
    QLabel *interfaceLabel = new QLabel("Interface:");
    inputsLayout->addWidget(interfaceLabel, INTERFACELABELROW, INTERFACELABELCOL);
    interfaceComboBox = new QComboBox;
    //create button to set network
    QLabel *networkLabel = new QLabel("Network (SSID, BSSID, Channel):");
    inputsLayout->addWidget(networkLabel, NETWORKSLABELROW, NETWORKSLABELCOL);
    networksComboBox = new QComboBox;
    //get and show interfaces and networks
    showInterfacesAndNetworks();
    //create button to refresh networks
    QPushButton *refreshButton = new QPushButton("Refresh Networks", this);
    inputsLayout->addWidget(refreshButton, REFRESHBUTTONROW, REFRESHBUTTONCOL);
    connect(refreshButton, SIGNAL (released()), this, SLOT (handleRefreshButton()));

    //buttons to select teams
    QLabel *teamATextLabel = new QLabel("TeamA:");
    QLabel *teamBTextLabel = new QLabel("TeamB:");
    comboBoxTeamA = new QComboBox;
    comboBoxTeamB = new QComboBox;
    //load existing teams
    for(uint i=0; i<AllTeams.size(); i++ )
    {
        comboBoxTeamA->addItem(AllTeams[i].name());
        comboBoxTeamB->addItem(AllTeams[i].name());
    }
    connect(comboBoxTeamA, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &gui::on_comboBoxTeamA_currentIndexChanged);
    connect(comboBoxTeamB, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &gui::on_comboBoxTeamB_currentIndexChanged);
    inputsLayout->addWidget(teamATextLabel, TEAMALABELROW, TEAMALABELCOL);
    inputsLayout->addWidget(comboBoxTeamA, TEAMACOMBOBOXROW, TEAMACOMBOBOXCOL);
    inputsLayout->addWidget(teamBTextLabel, TEAMBLABELROW, TEAMBLABELCOL);
    inputsLayout->addWidget(comboBoxTeamB, TEAMBCOMBOBOXROW, TEAMBCOMBOBOXCOL);

    //button to start game
    startButton = new QPushButton("Start Game", this);
    inputsLayout->addWidget(startButton, STARTBUTTONROW, STARTBUTTONCOL);
    connect(startButton, SIGNAL (released()), this, SLOT (toggleGame()));


    //new tab
    toolbox->addItem(inputsPage, tr("Click here for inputs"));

    //use grid
    gamePage = new QWidget;
    gameLayout = new QGridLayout(gamePage);

    //Tables with stations by team
    //Team A
    teamATableLabel = new QLabel(AllTeams[comboBoxTeamA->currentIndex()].name());
    gameLayout->addWidget(teamATableLabel, TEAMATABLELABELROW,0,1,2);
    teamResultView[TEAMA]= new QTableView();
    teamResultView[TEAMA]->setSortingEnabled(true);
    teamAStations = new QStandardItemModel();
    teamAStations->setColumnCount(NUMBEROFCOLUMNS);
    teamAStations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name" << "Packets" << "Packets/s" << "Throughput (Mb/s)" << "Power(dBm)" );
    teamResultView[TEAMA]->setModel(teamAStations);
    gameLayout->addWidget(teamResultView[0], TEAMATABLEROW,0,1,2);
    //Team B
    teamBTableLabel = new QLabel(AllTeams[comboBoxTeamB->currentIndex()].name());
    gameLayout->addWidget(teamBTableLabel, TEAMBTABLELABELROW,0,1,2);
    teamResultView[TEAMB]= new QTableView();
    teamResultView[TEAMB]->setSortingEnabled(true);
    teamBStations = new QStandardItemModel();
    teamBStations->setColumnCount(NUMBEROFCOLUMNS);
    teamBStations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name" << "Packets" << "Packets/s"<<  "Throughput (Mb/s)" <<"Power(dBm)" );
    teamResultView[TEAMB]->setModel(teamBStations);
    gameLayout->addWidget(teamResultView[1], TEAMBTABLEROW,0,1,2);


    //new tab
    toolbox->addItem(gamePage, tr("Click here for game view"));

    //use grid
    otherStationsPage = new QWidget;
    otherStationsLayout = new QGridLayout(otherStationsPage);
    //other stations
    teamNoneTableLabel = new QLabel("Other stations");
    otherStationsLayout->addWidget(teamNoneTableLabel, OTHERSTATIONSLABELROW,0,1,2);
    teamResultView[OTHERSTATIONS]= new QTableView();
    teamResultView[OTHERSTATIONS]->setSortingEnabled(true);
    otherStations = new QStandardItemModel();
    otherStations->setColumnCount(NUMBEROFOTHERCOLUMNS);
    otherStations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name"  << "Team Name"<< "Packets" << "Packets/s"<<  "Throughput (kb/s)"<< "Power(dBm)" );

    teamResultView[OTHERSTATIONS]->setModel(otherStations);
    otherStationsLayout->addWidget(teamResultView[OTHERSTATIONS], OTHERSTATIONSROW,0,1,2);

    warningLabel = new QLabel("");
    otherStationsLayout->addWidget(warningLabel, WARNINGSROW,0,1,2);

    toolbox->addItem(otherStationsPage, tr("Click here for other stations and alarms"));
}

/** @brief Returns game state
 *
 *  @return bool game state.
 *
 */
bool gui::isOnGame()
{
    return onGame;
}

/** @brief Starts game
 *
 *  Creates capture file. Changes network interface to monitor mode.
 *  Starts capture. Starts updating the display of statistics every second
 *
 *  @return Void.
 *
 */

void gui::startGame()
{
    if(proccessNetworkInformation()==false)
        return;

    QDateTime *curTime= new QDateTime(QDate::currentDate(), QTime::currentTime());
    QString game_name = QString::asprintf("%s_%s_%s",
                                          qPrintable(teamATableLabel->text().remove(" ")), qPrintable(teamBTableLabel->text().remove(" ")),
                                           qPrintable(curTime->toString("dd.MM.yyyy_hh:mm:ss")));
    wireless_obj.set_cap_file_prefix ("./log/" + game_name);
    if(cap_file_name!=NULL){
        delete cap_file_name;
    }
    cap_file_name = new QString(QString::asprintf("%s-01.csv", qPrintable("./log/" + game_name)));
    delete curTime;

    wireless_obj.start_iw_monitor_mode();
    wireless_obj.start_capture();
    AllTeams[comboBoxTeamA->currentIndex()].set_inGame();
    AllTeams[comboBoxTeamB->currentIndex()].set_inGame();
    startButton->setText("Stop Game");

    //refresh every second
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &gui::display);
    onGame = true;
    ROBOLOG_NOTICE("%s:%s", qPrintable(GAME_START),qPrintable(game_name));
}

/** @brief Stops game
 *
 *  Stop capture. Leave monitor mode. Stops updating statistics.
 *  Cleans all players and teams statistics
 *
 *  @return Void.
 *
 */
void gui::stopGame()
{
    wireless_obj.stop_capture();
    QStringList *iw_list = wireless_obj.get_wl_interfaces();
    wireless_obj.stop_all_mon_interfaces(*iw_list);
    delete iw_list;
    delete timer;
    delete cap_file_name;
    cap_file_name = NULL;
    startButton->setText("Start Game");
    clean_AllPlayers_stat();
    clean_AllTeam_stat();
    onGame = false;
    ROBOLOG_NOTICE("%s", GAME_END);
}

/** @brief Refresh network list
 *
 *  Get networks list and displays it on combo box
 *
 *  @return Void.
 *
 */
void gui::refreshAndShowNetworks()
{
    if(isOnGame() == true)
    {
        QMessageBox::warning(this,
            tr("Invalid!"),
            tr("Stop game") );
        return;
    }

    delete networksComboBox;
    networksComboBox = new QComboBox;

    networks_list = wireless_obj.get_wl_networks();

    foreach (const QString &str, *networks_list)
    {
        networksComboBox->addItem(str);
    }

    inputsLayout->addWidget(networksComboBox, NETWORKSCOMBOBOXROW, NETWORKSCOMBOBOXCOL);
}

/** @brief Refresh interfaces and networks list
 *
 *  Get interfaces list and displays it on combo box.
 *  Get networks list and displays it on combo box.
 *
 *  @return Void.
 *
 */
void gui::showInterfacesAndNetworks()
{
    delete interfaceComboBox;
    interfaceComboBox = new QComboBox;

    iw_list = wireless_obj.get_wl_interfaces();
    foreach (const QString &str, *iw_list)
    {
        interfaceComboBox->addItem(str);
    }

    inputsLayout->addWidget(interfaceComboBox, INTERFACECOMBOBOXROW, INTERFACECOMBOBOXCOL);

    wireless_obj.set_cap_wl_interface(iw_list->at(interfaceComboBox->currentIndex()));
    refreshAndShowNetworks();
    connect(interfaceComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &gui::on_interfaceComboBox_currentIndexChanged);
}

/** @brief Refresh networks list when interface is changed
 *
 *  Get networks list of the selected interface and displays it on combo box.
 *
 *  @return Void.
 *
 */
void gui::on_interfaceComboBox_currentIndexChanged()
{    
    if(isOnGame() == true)
    {
        QMessageBox::warning(this,
            tr("Invalid!"),
            tr("Stop game") );
        return;
    }
    wireless_obj.set_cap_wl_interface(iw_list->at(interfaceComboBox->currentIndex()));
    refreshAndShowNetworks();
}

/** @brief Refresh networks list
 *
 *  Get networks list and displays it on combo box.
 *
 *  @return Void.
 *
 */
void gui::handleRefreshButton()
{
    refreshAndShowNetworks();
}

/** @brief Toggle game state
 *
 *  Starts game if not on game.
 *  Stops game if on game.
 *
 *  @return Void.
 *
 */
void gui::toggleGame()
{

    if(isOnGame()==true)
    {
        stopGame();
    }
    else
    {
        startGame();
    }

    inputsLayout->addWidget(startButton, STARTBUTTONROW, STARTBUTTONCOL);
}

/** @brief Proccess network information
 *
 *  Parses network information to get the bssid and channel information
 *
 *  @return bool true if success, false if error
 *
 */
bool gui::proccessNetworkInformation()
{
    if(networksComboBox->currentText().isEmpty()==true)
    {
        QMessageBox::warning(this,
            tr("Error!"),
            tr("No network selected") );
        return false;
    }
    QStringList networkParam = networksComboBox->currentText().split(",",QString::SkipEmptyParts);

    if(networkParam.size()!=3)
    {
        QMessageBox::warning(this,
            tr("Error!"),
            tr("Error proccessing selected network") );
        return false;
    }
    wireless_obj.set_cap_bssid(networkParam.at(1));
    wireless_obj.set_cap_channel(networkParam.at(2).toInt());
    return true;
}

/** @brief Change team A
 *
 *  If not on game, changes the A team
 *
 *  @return Void
 *
 */
void gui::on_comboBoxTeamA_currentIndexChanged()
{
    if(isOnGame() == true)
    {
        QMessageBox::warning(this,
             tr("Error"),
             tr("Restart game to apply") );
        return;
    }
    delete teamATableLabel;
    teamATableLabel = new QLabel(AllTeams[comboBoxTeamA->currentIndex()].name());
    gameLayout->addWidget(teamATableLabel,TEAMATABLELABELROW, 0,1,2);
}

/** @brief Change team B
 *
 *  If not on game, changes the B team
 *
 *  @return Void
 *
 */
void gui::on_comboBoxTeamB_currentIndexChanged()
{
    if(isOnGame() == true)
    {
        QMessageBox::warning(this,
             tr("Error"),
             tr("Restart game to apply") );
        return;
    }
    delete teamBTableLabel;
    teamBTableLabel = new QLabel(AllTeams[comboBoxTeamB->currentIndex()].name());
    gameLayout->addWidget(teamBTableLabel, TEAMBTABLELABELROW,0,1,2);
}

/** @brief updates data display
 *
 *  Updates statistics
 *
 *  @return Void
 *
 */
void gui::display()
{
    if(cap_file_name!=NULL){
        parseNetCapture(*cap_file_name);
    }
    QString teamTh;

    delete teamATableLabel;
    if(AllTeams[comboBoxTeamA->currentIndex()].throughput()<128 && AllTeams[comboBoxTeamA->currentIndex()].throughput()!=0)
        teamTh = QString::asprintf("%s <0.001 Mbs",qPrintable(AllTeams[comboBoxTeamA->currentIndex()].name()));
    else
        teamTh = QString::asprintf("%s %f Mbs",qPrintable(AllTeams[comboBoxTeamA->currentIndex()].name()), ((float)AllTeams[comboBoxTeamA->currentIndex()].throughput())/131072);

    teamATableLabel = new QLabel(teamTh);
    gameLayout->addWidget(teamATableLabel, TEAMATABLELABELROW,0,1,2);

    //Team A
    //first delete rows
    int teamARows=teamAStations->rowCount();
    for(int i=0; i<teamARows;i++)
    {
        teamAStations->removeRow(teamARows-1-i);
    }
    //add rows
    int row_cnt = 0;
    for(int i=0;i<AllTeams[comboBoxTeamA->currentIndex()].get_team_size();i++)
    {
        player *teamAPlayer;
        AllTeams[comboBoxTeamA->currentIndex()].get_player(i,&teamAPlayer);
        if(teamAPlayer->packets()>0){
            teamAStations->insertRow(row_cnt, addPlayerToList(teamAPlayer, false));
            row_cnt++;
        }
    }
    updateTable(TEAMA);

    //Team B
    //first delete rows
    int teamBRows=teamBStations->rowCount();
    for(int i=0; i<teamBRows;i++)
    {
        teamBStations->removeRow(teamBRows-1-i);
    }
    delete teamBTableLabel;
    if(AllTeams[comboBoxTeamB->currentIndex()].throughput()<128 && AllTeams[comboBoxTeamB->currentIndex()].throughput()!=0)
        teamTh = QString::asprintf("%s <0.001 Mbs",qPrintable(AllTeams[comboBoxTeamB->currentIndex()].name()));
    else
        teamTh = QString::asprintf("%s %f Mbs",qPrintable(AllTeams[comboBoxTeamB->currentIndex()].name()), ((float)AllTeams[comboBoxTeamB->currentIndex()].throughput())/131072);

    teamBTableLabel = new QLabel(teamTh);
    gameLayout->addWidget(teamBTableLabel, TEAMBTABLELABELROW,0,1,2);
    //add rows
    row_cnt = 0;
    for(int i=0;i<AllTeams[comboBoxTeamB->currentIndex()].get_team_size();i++)
    {
        player *teamBPlayer;
        AllTeams[comboBoxTeamB->currentIndex()].get_player(i, &teamBPlayer);
        if(teamBPlayer->packets()>0){
            teamBStations->insertRow(row_cnt, addPlayerToList(teamBPlayer, false));
            row_cnt++;
        }
    }
    updateTable(TEAMB);

    //other stations
    //first delete rows
    int othertRows=otherStations->rowCount();
    for(int i=0; i<othertRows;i++)
    {
        otherStations->removeRow(othertRows-1-i);
    }
    //add rows
    int counter=0;//numberofplayers
    for(uint i=0;i< AllPlayers.size();i++)
    {
        if(AllPlayers[i]->isConnected())
        {
            if(AllPlayers[i]->team_name() != NULL)
            {
                if((AllPlayers[i]->team_name() == AllTeams[comboBoxTeamA->currentIndex()].name()) || (AllPlayers[i]->team_name() == AllTeams[comboBoxTeamB->currentIndex()].name()))
                {
                    continue;
                }
            }
            otherStations->insertRow(counter, addPlayerToList(AllPlayers[i], true));
            counter++;
        }
    }
    updateTable(OTHERSTATIONS);

    QString *warning_output = new QString("");

    //check alarm warnings
    if(AllTeams[comboBoxTeamA->currentIndex()].bwAlarmed() == true || AllTeams[comboBoxTeamB->currentIndex()].bwAlarmed() == true)
    {
        warning_output->append("WARNING: too much bandwith is occupied by:\n");
        if(AllTeams[comboBoxTeamA->currentIndex()].bwAlarmed() == true)
        {
            warning_output->append(AllTeams[comboBoxTeamA->currentIndex()].name());
            warning_output->append("\n");

        }
        if(AllTeams[comboBoxTeamB->currentIndex()].bwAlarmed() == true)
        {
            warning_output->append(AllTeams[comboBoxTeamB->currentIndex()].name());
        }
    }
    delete warningLabel;
    warningLabel = new QLabel(*warning_output);
    delete warning_output;
    otherStationsLayout->addWidget(warningLabel, WARNINGSROW,0,1,2);

}

/** @brief Parses player information to list item
 *
 *  Parses player information to list item so that it can be added to players list
 *
 *
 *  @param player_to_add pointer to player to be added
 *  @param showTeam boolean that states if team information should be added or not
 *  @return standardItemsList item to be added to list
 *
 */
QList<QStandardItem *> gui::addPlayerToList(player *player_to_add, bool showTeam)
{
    QList<QStandardItem *> standardItemsList;

    standardItemsList.append(new QStandardItem(player_to_add->mac()));
    standardItemsList.append(new QStandardItem(player_to_add->firstTimeSeen().toString("yyyy-MM-dd hh:mm:ss")));
    standardItemsList.append(new QStandardItem(player_to_add->lastTimeSeen().toString("yyyy-MM-dd hh:mm:ss")));
    standardItemsList.append(new QStandardItem(player_to_add->name()));

    if(showTeam)
    {
        standardItemsList.append(new QStandardItem(player_to_add->team_name()));
    }
    standardItemsList.append(new QStandardItem(QString::number(player_to_add->packets())));
    standardItemsList.append(new QStandardItem(QString::number(player_to_add->pkts_second())));

    //convert bytes to kbits (*8/1024/1024=/131072)
    if(player_to_add->throughput()<128 && player_to_add->throughput()!=0)
        standardItemsList.append(new QStandardItem("<0.001"));
    else
        standardItemsList.append(new QStandardItem(QString::number(((float)player_to_add->throughput())/131072)));

    if(player_to_add->power()!=std::numeric_limits<int>::min())
    {
        standardItemsList.append(new QStandardItem(QString::number(player_to_add->power())));
    }
    else
    {
        standardItemsList.append(new QStandardItem("-INF"));
    }

    return standardItemsList;
}

/** @brief updates team table information
 *
 *  Updates display settings. Displays information
 *
 *  @param tableIndex table to be updated
 *  @return Void
 *
 */
void gui::updateTable(int tableIndex)
{
    teamResultView[tableIndex]->setWordWrap(true);
    if(tableIndex==OTHERSTATIONS)
    {
        for(int i = 0; i<NUMBEROFOTHERCOLUMNS; i++)
        {
            teamResultView[tableIndex]->resizeColumnToContents(i);
        }
        teamResultView[tableIndex]->horizontalHeader()->setStretchLastSection(true);

        otherStationsLayout->addWidget(teamResultView[tableIndex], 1,0,1,2);
    }
    else
    {
        for(int i = 0; i<NUMBEROFCOLUMNS; i++)
        {
            teamResultView[tableIndex]->resizeColumnToContents(i);
        }
        teamResultView[tableIndex]->horizontalHeader()->setStretchLastSection(true);
        gameLayout->addWidget(teamResultView[tableIndex], tableIndex*2+1,0,1,2);
    }
}

/** @brief Change game teams
 *
 *
 *  @param comboBoxIndex 1-TeamA 2-TeamB
 *  @param teamName
 *  @return Void
 *
 */
void gui::changeGameTeams(int comboBoxIndex, QString teamName)
{
    //check if the team is on combobox
    int index=comboBoxTeamA->findText(teamName);
    if(index == -1)
    {
        qDebug("Couldn't change game team. Team %s wasn't found.", qPrintable(teamName));
        return;
    }

    if(comboBoxIndex==1)
    {
        comboBoxTeamA->setCurrentIndex(index);
    }
    else if(comboBoxIndex==2)
    {
        comboBoxTeamB->setCurrentIndex(index);
    }
    else
    {
        qDebug("Couldn't change game team. Invalid index %d was found.", comboBoxIndex);
    }
}
