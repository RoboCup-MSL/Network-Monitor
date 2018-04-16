#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QBoxLayout>
#include <QApplication>
#include <QTableWidget>
#include <QtWidgets>
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
        QMessageBox::warning(
            this,
            tr("Error!"),
            tr("There are no teams!") );
        return;
    }
    //initialize     airodump
    airodump = new QProcess(this);

    //this will be the base layout
    QVBoxLayout *baseLayout;
    baseLayout = new QVBoxLayout(this);

    //Question: is toolbox the best solution?
    toolbox = new QToolBox;
    toolbox->setMinimumSize(750, 500);
    baseLayout->addWidget(toolbox);

    //use grid
    page = new QWidget;
    layout = new QGridLayout(page);

    //Button 1
    //button to get and set network name
    int frameStyle = QFrame::Sunken | QFrame::Panel;
    networkNameLabel = new QLabel;
    networkNameLabel->setFrameStyle(frameStyle);
    QPushButton *networkNameButton = new QPushButton(tr("Click to insert network BSSID:"));
    layout->addWidget(networkNameButton, 0, 0);
    layout->addWidget(networkNameLabel, 0, 1);

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
    layout->addWidget(channelTextLabel, 1, 0);
    layout->addWidget(comboBoxWifiChannel, 1, 1);

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

    layout->addWidget(teamATextLabel, 2, 0);
    layout->addWidget(comboBoxTeamA, 2, 1);
    layout->addWidget(teamBTextLabel, 3, 0);
    layout->addWidget(comboBoxTeamB, 3, 1);

    //Tables with stations by team
    //Note: I should have done it in a function
    //first I need the current time

    //Team A
    teamATableLabel = new QLabel(AllTeams[comboBoxTeamA->currentIndex()].name());
    QStandardItemModel *teamAStations = new QStandardItemModel();
    QTableView *teamAResultView = new QTableView();
    teamAResultView->setModel(teamAStations);
    teamAResultView->horizontalHeader()->setStretchLastSection(true);

    teamAResultView->setWordWrap(true);
    teamAResultView->verticalHeader()->hide();
    teamAResultView->setSortingEnabled(true);
    teamAStations->setColumnCount(4);
    teamAStations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name");

    for(int i=0;i<AllTeams[comboBoxTeamA->currentIndex()].get_team_size();i++)
    {
        player *teamAPlayer;
        QList<QStandardItem *> standardItemsList;

        AllTeams[comboBoxTeamA->currentIndex()].get_player(i,&teamAPlayer);
        standardItemsList.append(new QStandardItem(teamAPlayer->mac()));
        standardItemsList.append(new QStandardItem(teamAPlayer->firstTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamAPlayer->lastTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamAPlayer->name()));
        //standardItemsList.setStyleSheet("QListView { background-color: #80FF80 }");
        teamAStations->insertRow(teamAStations->rowCount(), standardItemsList);

    }
    teamAResultView->resizeColumnToContents(0);
    teamAResultView->resizeColumnToContents(1);
    teamAResultView->resizeColumnToContents(2);
    teamAResultView->resizeColumnToContents(3);
    teamAResultView->sortByColumn(2, Qt::DescendingOrder);

    layout->addWidget(teamATableLabel, 4,0,1,2);
    layout->addWidget(teamAResultView, 5,0,1,2);

    //Team B
    teamBTableLabel = new QLabel(AllTeams[comboBoxTeamB->currentIndex()].name());
    QStandardItemModel *teamBStations = new QStandardItemModel();
    QTableView *teamBResultView = new QTableView();
    teamBResultView->setModel(teamBStations);
    teamBResultView->horizontalHeader()->setStretchLastSection(true);
    teamBResultView->setSortingEnabled(true);

    teamBStations->setColumnCount(4);
    teamBStations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name"  );
    teamBResultView->verticalHeader()->hide();

    for(int i=0;i<AllTeams[comboBoxTeamB->currentIndex()].get_team_size();i++)
    {
        player *teamBPlayer;
        QList<QStandardItem *> standardItemsList;

        AllTeams[comboBoxTeamB->currentIndex()].get_player(i,&teamBPlayer);
        standardItemsList.append(new QStandardItem(teamBPlayer->mac()));
        standardItemsList.append(new QStandardItem(teamBPlayer->firstTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamBPlayer->lastTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamBPlayer->name()));

        teamBStations->insertRow(teamBStations->rowCount(), standardItemsList);
    }
    teamBResultView->resizeColumnToContents(0);
    teamBResultView->resizeColumnToContents(1);
    teamBResultView->resizeColumnToContents(2);
    teamBResultView->resizeColumnToContents(3);
    teamBResultView->sortByColumn(2, Qt::DescendingOrder);

    layout->addWidget(teamBTableLabel, 6,0,1,2);
    layout->addWidget(teamBResultView, 7,0,1,2);

    toolbox->addItem(page, tr("Game"));

    //refresh every second
    timer = new QTimer(this);
    timer->setInterval(750);
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
        networkName=new QString(text);
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
    layout->addWidget(teamATableLabel, 4,0,1,2);
    processAirodump();
}

void gui::on_comboBoxTeamB_currentIndexChanged()
{
    delete teamBTableLabel;
    teamBTableLabel = new QLabel(AllTeams[comboBoxTeamB->currentIndex()].name());
    layout->addWidget(teamBTableLabel, 6,0,1,2);
    processAirodump();
}

void gui::display()
{
    if(cap_file_name!=NULL){
        parseNetCapture(*cap_file_name);
    }

    //Team A
    QStandardItemModel *teamAStations = new QStandardItemModel();
    QTableView *teamAResultView = new QTableView();
    teamAResultView->setModel(teamAStations);
    teamAResultView->setWordWrap(1);

    teamAStations->setColumnCount(4);
    teamAStations->setHorizontalHeaderLabels(QStringList() << "MAC" << "First Seen" << "Last Seen" << "Name");
    teamAResultView->horizontalHeader()->setStretchLastSection(true);
    teamAResultView->setWordWrap(true);
    teamAResultView->verticalHeader()->hide();
    teamAResultView->setSortingEnabled(true);

    for(int i=0;i<AllTeams[comboBoxTeamA->currentIndex()].get_team_size();i++)
    {
        player *teamAPlayer;
        QList<QStandardItem *> standardItemsList;

        AllTeams[comboBoxTeamA->currentIndex()].get_player(i,&teamAPlayer);
        standardItemsList.append(new QStandardItem(teamAPlayer->mac()));
        standardItemsList.append(new QStandardItem(teamAPlayer->firstTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamAPlayer->lastTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamAPlayer->name()));

        teamAStations->insertRow(teamAStations->rowCount(), standardItemsList);
    }

    teamAResultView->resizeColumnToContents(0);
    teamAResultView->resizeColumnToContents(1);
    teamAResultView->resizeColumnToContents(2);
    teamAResultView->resizeColumnToContents(3);
    teamAResultView->sortByColumn(2, Qt::DescendingOrder);

    layout->addWidget(teamAResultView, 5,0,1,2);

    //Team B
    QStandardItemModel *teamBStations = new QStandardItemModel();
    QTableView *teamBResultView = new QTableView();
    teamBResultView->setModel(teamBStations);
    teamBResultView->setSortingEnabled(true);

    teamBStations->setColumnCount(4);
    teamBStations->setHorizontalHeaderLabels(QStringList()  << "MAC"<< "First Seen" << "Last Seen"<< "Name" );
    teamBResultView->horizontalHeader()->setStretchLastSection(true);
    teamBResultView->verticalHeader()->hide();

    for(int i=0;i<AllTeams[comboBoxTeamB->currentIndex()].get_team_size();i++)
    {
        player *teamBPlayer;
        QList<QStandardItem *> standardItemsList;

        AllTeams[comboBoxTeamB->currentIndex()].get_player(i, &teamBPlayer);
        standardItemsList.append(new QStandardItem(teamBPlayer->mac()));
        standardItemsList.append(new QStandardItem(teamBPlayer->firstTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamBPlayer->lastTimeSeen().toString()));
        standardItemsList.append(new QStandardItem(teamBPlayer->name()));

        teamBStations->insertRow(teamBStations->rowCount(), standardItemsList);
    }
    teamBResultView->resizeColumnToContents(0);
    teamBResultView->resizeColumnToContents(1);
    teamBResultView->resizeColumnToContents(2);
    teamBResultView->resizeColumnToContents(3);
    teamBResultView->sortByColumn(2, Qt::DescendingOrder);

    layout->addWidget(teamBResultView, 7,0,1,2);
}
