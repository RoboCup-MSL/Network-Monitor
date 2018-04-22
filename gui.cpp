#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QBoxLayout>
#include <QApplication>
#include <QTableWidget>
#include "gui.h"
#include "team.h"
#include "robocop_funcs.h"
extern vector<team>     AllTeams;

int channelList[NUMBEROFWIFICHANNELS]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,34,36,38,40,42,44,46,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161};

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

    //initialize     airodump
    airodump = new QProcess(this);

    //this will be the base layout
    QHBoxLayout *baseLayout;
    baseLayout = new QHBoxLayout(this);

    //Question: is toolbox the best solution?
    toolbox = new QToolBox;
    toolbox->setMinimumSize(750, 500);
    baseLayout->addWidget(toolbox);

    //use grid
    inputsPage = new QWidget;
    inputsLayout = new QGridLayout(inputsPage);

    //Button 1
    //button to get and set network name
    int frameStyle = QFrame::Sunken | QFrame::Panel;
    networkNameLabel = new QLabel;
    networkNameLabel->setFrameStyle(frameStyle);
    QPushButton *networkNameButton = new QPushButton(tr("Click to insert network BSSID:"));
    inputsLayout->addWidget(networkNameButton, 0, 0);
    inputsLayout->addWidget(networkNameLabel, 0, 1);

    //connect to an action when we receive the input
    connect(networkNameButton, &QAbstractButton::clicked, this, &gui::setNetworkName);

    //Button 2
    //button to select the wifi channel
    QLabel *channelTextLabel = new QLabel("Choose 802.11 channel:");
    comboBoxWifiChannel = new QComboBox;

    for(int i = 0; i < NUMBEROFWIFICHANNELS; i++)
    {
        comboBoxWifiChannel->addItem(" " + QString::number(channelList[i]));
    }

    connect(comboBoxWifiChannel, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &gui::on_comboBoxWifiChannel_currentIndexChanged);
    inputsLayout->addWidget(channelTextLabel, 1, 0);
    inputsLayout->addWidget(comboBoxWifiChannel, 1, 1);

    //buttons 3 and 4
    //select teams
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

    inputsLayout->addWidget(teamATextLabel, 2, 0);
    inputsLayout->addWidget(comboBoxTeamA, 2, 1);
    inputsLayout->addWidget(teamBTextLabel, 3, 0);
    inputsLayout->addWidget(comboBoxTeamB, 3, 1);

    toolbox->addItem(inputsPage, tr("Click here for inputs"));

    //use grid
    gamePage = new QWidget;
    gameLayout = new QGridLayout(gamePage);

    //Tables with stations by team
    //Team A
    teamATableLabel = new QLabel(AllTeams[comboBoxTeamA->currentIndex()].name());
    gameLayout->addWidget(teamATableLabel, 0,0,1,2);
    teamResultView[0]= new QTableView();
    gameLayout->addWidget(teamResultView[0], 1,0,1,2);


    //Team B
    teamBTableLabel = new QLabel(AllTeams[comboBoxTeamB->currentIndex()].name());
    gameLayout->addWidget(teamBTableLabel, 2,0,1,2);
    teamResultView[1]= new QTableView();
    gameLayout->addWidget(teamResultView[1], 3,0,1,2);

    //None
    teamNoneTableLabel = new QLabel("Other stations");
    gameLayout->addWidget(teamNoneTableLabel, 4,0,1,2);
    teamResultView[2]= new QTableView();
    gameLayout->addWidget(teamResultView[2], 5,0,1,2);

    toolbox->addItem(gamePage, tr("Click here for game"));

    //refresh every second
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &gui::display);
}

bool isValidMacAddress(const char* mac) {
    int i = 0;
    int s = 0;

    while (*mac) {
        if (std::isxdigit(*mac)) {
            i++;
        } else if (*mac == ':' || *mac == '-') {
            if (i == 0 || i / 2 - 1 != s)
                break;
            ++s;
        }
        else {
            s = -1;
        }
        ++mac;
    }
    return (i == 12 && (s == 5 || s == 0));
}

void gui::setNetworkName()
{
    bool okButton;
    bool isValidMac;

    QString text= QInputDialog::getText(this, tr("Game Network"),
                                         tr("Insert BSSID:\n(BSSID is the MAC address of the wireless access point)"), QLineEdit::Normal,
                                         "", &okButton);

    isValidMac = isValidMacAddress(text.toStdString().c_str());

    if (okButton && !text.isEmpty() && isValidMac)
    {
        networkName=new QString(text.trimmed());
        networkNameLabel->setText(text);
    }
    if(!isValidMac)
    {
        QMessageBox::warning(
            this,
            tr("Error!"),
            tr("Not a valid BSSID.") );
        return;
    }
    processAirodump();

}

void gui::on_comboBoxWifiChannel_currentIndexChanged()
{
    processAirodump();
}

void gui::processAirodump()
{
    //check if we can start airodump
    if(networkName==NULL)
        return;

    //check the airodump state
    if(airodump->state()!=QProcess::NotRunning)
    {
        airodump->kill();
        if(airodump->waitForFinished(1000)!=true)
        {
            qDebug("Couldn't kill airodump");
            return;
        }
    }
    QDateTime *curTime= new QDateTime(QDate::currentDate(), QTime::currentTime());
    QString command = QString::asprintf("sudo airodump-ng -c %d -M -d %s -w %s_%s_%s --output-format csv mon0",
                                  channelList[comboBoxWifiChannel->currentIndex()] , qPrintable(*networkName),
                                   qPrintable(teamATableLabel->text().remove(" ")), qPrintable(teamBTableLabel->text().remove(" ")),
                                    qPrintable(curTime->toString("dd.MM.yyyy_hh:mm:ss")));
    if(cap_file_name!=NULL){
        delete cap_file_name;
    }
    cap_file_name = new QString(QString::asprintf("%s_%s_%s-01.csv", qPrintable(teamATableLabel->text().remove(" ")), qPrintable(teamBTableLabel->text().remove(" ")),
                                      qPrintable(curTime->toString("dd.MM.yyyy_hh:mm:ss"))));
    airodump->start(command);
    if(airodump->waitForStarted()!=true)
    {
        qDebug("Couldn't start airodump");
        return;
    }

}

void gui::on_comboBoxTeamA_currentIndexChanged()
{
    delete teamATableLabel;
    teamATableLabel = new QLabel(AllTeams[comboBoxTeamA->currentIndex()].name());
    gameLayout->addWidget(teamATableLabel, 0,0,1,2);
    processAirodump();
}

void gui::on_comboBoxTeamB_currentIndexChanged()
{
    delete teamBTableLabel;
    teamBTableLabel = new QLabel(AllTeams[comboBoxTeamB->currentIndex()].name());
    gameLayout->addWidget(teamBTableLabel, 2,0,1,2);
    processAirodump();
}

void gui::display()
{
    if(cap_file_name!=NULL){
        parseNetCapture(*cap_file_name);
    }

    team *other;
    other = get_team_by_name(team_none_name);
    print_team(other);

    //Team A
    QStandardItemModel *teamAStations = new QStandardItemModel();

    for(int i=0;i<AllTeams[comboBoxTeamA->currentIndex()].get_team_size();i++)
    {
        player *teamAPlayer;

        AllTeams[comboBoxTeamA->currentIndex()].get_player(i,&teamAPlayer);
        teamAStations->insertRow(i, addPlayerToList(teamAPlayer, false));
    }
    updateTable(teamAStations, 0);

    //Team B
    QStandardItemModel *teamBStations = new QStandardItemModel();

    for(int i=0;i<AllTeams[comboBoxTeamB->currentIndex()].get_team_size();i++)
    {
        player *teamBPlayer;

        AllTeams[comboBoxTeamB->currentIndex()].get_player(i, &teamBPlayer);
        teamBStations->insertRow(i, addPlayerToList(teamBPlayer, false));
    }
    updateTable(teamBStations, 1);


    team* teamNone;
    if((teamNone=get_team_by_name(team_none_name))!=NULL)
    {
        QStandardItemModel *teamNoneStations = new QStandardItemModel();

        for(int i=0;i<teamNone->get_team_size();i++)
        {
            player *teamNonePlayer;

            teamNone->get_player(i,&teamNonePlayer);
            teamNoneStations->insertRow(i, addPlayerToList(teamNonePlayer, true));
        }
        updateTable(teamNoneStations, 2);
    }
}

QList<QStandardItem *> gui::addPlayerToList(player *player_to_add, bool showTeam)
{
    QList<QStandardItem *> standardItemsList;

    standardItemsList.append(new QStandardItem(player_to_add->mac()));
    standardItemsList.append(new QStandardItem(player_to_add->firstTimeSeen().toString("yyyy-MM-dd hh:mm:ss")));
    standardItemsList.append(new QStandardItem(player_to_add->lastTimeSeen().toString("yyyy-MM-dd hh:mm:ss")));
    standardItemsList.append(new QStandardItem(player_to_add->name()));
    if(showTeam)
        standardItemsList.append(new QStandardItem(player_to_add->team_name()));

    standardItemsList.append(new QStandardItem(QString::number(player_to_add->packets())));
    standardItemsList.append(new QStandardItem(QString::number(player_to_add->pkts_second())));

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

void gui::updateTable(QStandardItemModel *stations, int tableIndex)
{

    //if it's the others table we need to and a column to team name
    if(tableIndex==2)
    {
        stations->setColumnCount(7);
        stations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name"  << "Team Name"<< "Packets" << "Packets/sec"<< "Power(dBm)" );

        teamResultView[tableIndex]->setModel(stations);
        teamResultView[tableIndex]->resizeColumnToContents(0);
        teamResultView[tableIndex]->resizeColumnToContents(1);
        teamResultView[tableIndex]->resizeColumnToContents(2);
        teamResultView[tableIndex]->resizeColumnToContents(3);
        teamResultView[tableIndex]->resizeColumnToContents(4);
        teamResultView[tableIndex]->resizeColumnToContents(5);
        teamResultView[tableIndex]->resizeColumnToContents(6);
        teamResultView[tableIndex]->resizeColumnToContents(7);

    }
    else
    {
        stations->setColumnCount(6);
        stations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name"  << "Packets" << "Packets/sec"<< "Power(dBm)" );
        teamResultView[tableIndex]->setModel(stations);
        teamResultView[tableIndex]->resizeColumnToContents(0);
        teamResultView[tableIndex]->resizeColumnToContents(1);
        teamResultView[tableIndex]->resizeColumnToContents(2);
        teamResultView[tableIndex]->resizeColumnToContents(3);
        teamResultView[tableIndex]->resizeColumnToContents(4);
        teamResultView[tableIndex]->resizeColumnToContents(5);
        teamResultView[tableIndex]->resizeColumnToContents(6);
    }
    teamResultView[tableIndex]->horizontalHeader()->setStretchLastSection(true);
    //teamResultView[tableIndex]->sortByColumn(2, Qt::DescendingOrder);
    gameLayout->addWidget(teamResultView[tableIndex], tableIndex*2+1,0,1,2);
}

//comboBoxIndex: 1-TeamA 2-TeamB
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
