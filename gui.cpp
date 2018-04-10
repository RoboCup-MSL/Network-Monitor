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

int channelList[NUMBEROFWIFICHANNELS]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,34,36,38,40,42,44,46,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161};

gui::gui(QWidget *parent) : QWidget(parent)
{
    //this will be the base layout
    QVBoxLayout *baseLayout;
    baseLayout = new QVBoxLayout(this);

    //Question: is toolbox the best solution?
    toolbox = new QToolBox;
    toolbox->setMinimumSize(500, 250);
    baseLayout->addWidget(toolbox);

    //use grid
    page = new QWidget;
    layout = new QGridLayout(page);

    //Button 1
    //button to get and set network name
    int frameStyle = QFrame::Sunken | QFrame::Panel;
    networkNameLabel = new QLabel;
    networkNameLabel->setFrameStyle(frameStyle);
    QPushButton *networkNameButton = new QPushButton(tr("Click to insert network name"));
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

    //TODO load existing teams
    for(int i = 0; i < 5; i++)
    {
        comboBoxTeamA->addItem("Equipa " + QString::number(i));
        comboBoxTeamB->addItem("Equipa " + QString::number(i));

    }
    connect(comboBoxTeamA, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &gui::on_comboBoxTeamA_currentIndexChanged);
    connect(comboBoxTeamB, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &gui::on_comboBoxTeamB_currentIndexChanged);

    layout->addWidget(teamATextLabel, 2, 0);
    layout->addWidget(comboBoxTeamA, 2, 1);
    layout->addWidget(teamBTextLabel, 3, 0);
    layout->addWidget(comboBoxTeamB, 3, 1);

    //codigo de teste da tabela e do timer
    queryLabelStations = new QLabel(QApplication::translate("Policia", "Stations"));
    QFile fileStations("./team/minon-02.csv");

    QTextStream in(&fileStations);
    QString line;
    QStandardItemModel *csvModelStations = new QStandardItemModel();

    QTableView *resultViewStations = new QTableView();
    resultViewStations->setModel(csvModelStations);

    csvModelStations->setColumnCount(5);
    csvModelStations->setHorizontalHeaderLabels(QStringList() << "MAC" <<"First Seen"<<"Last Seen" <<"POWER"<< "Packets");

    if ( !fileStations.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File minon-02 does not exists ";
    } else {
        QString lineToSkip;

        line = in.readLine();
        int aux=0;
        do{
           qDebug() << "Skipping line=" << lineToSkip <<endl;
           lineToSkip = in.readLine();

           continue;
        } while((lineToSkip.indexOf("Station MAC")!=0)&& (!in.atEnd()));
        do{
            line = in.readLine();
            QList<QStandardItem *> standardItemsList;

            foreach (QString item, line.split(","))
            {
                aux++;
                qDebug() << "line split=" << aux <<endl;

                standardItemsList.append(new QStandardItem(item));
            }
            csvModelStations->insertRow(csvModelStations->rowCount(), standardItemsList);
        }        while (!in.atEnd());

    }

    layout->addWidget(queryLabelStations, 4,0,1,2);
    layout->addWidget(resultViewStations, 5,0,1,2);
    toolbox->addItem(page, tr("Game"));

    fileStations.close();
    qDebug()<<"mainwindow timer 1"<<endl;
    timer = new QTimer(this);
    timer->setInterval(750);
    timer->start(1000);
    qDebug()<<"mainwindow timer 2"<<endl;
    connect(timer, &QTimer::timeout, this, &gui::display);
}

void gui::setNetworkName()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("User name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
        networkNameLabel->setText(text);
}

void gui::on_comboBoxWifiChannel_currentIndexChanged()
{
    //QMessageBox::information(this, "Item Selection",
    //                             ComboBoxWifiChannel->currentText());
}

void gui::on_comboBoxTeamA_currentIndexChanged()
{

}

void gui::on_comboBoxTeamB_currentIndexChanged()
{

}

void gui::display()
{
    QFile fileStations("./team/minon-02.csv");

    QTextStream in(&fileStations);
    QString line;
    QStandardItemModel *csvModelStations = new QStandardItemModel();

    QTableView *resultViewStations = new QTableView();
    resultViewStations->setModel(csvModelStations);

    csvModelStations->setColumnCount(5);
    csvModelStations->setHorizontalHeaderLabels(QStringList() << "MAC" <<"First Seen"<<"Last Seen" <<"POWER"<< "Packets");

    if ( !fileStations.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File minon-02 does not exists ";
    } else {
        QString lineToSkip;

        line = in.readLine();
        int aux=0;
        do{
           qDebug() << "Skipping line=" << lineToSkip <<endl;
           lineToSkip = in.readLine();

           continue;
        } while((lineToSkip.indexOf("Station MAC")!=0)&& (!in.atEnd()));
        do{
            line = in.readLine();
            QList<QStandardItem *> standardItemsList;

            foreach (QString item, line.split(","))
            {
                aux++;
                qDebug() << "line split=" << aux <<endl;

                standardItemsList.append(new QStandardItem(item));
            }
            csvModelStations->insertRow(csvModelStations->rowCount(), standardItemsList);
        }        while (!in.atEnd());

    }

    layout->addWidget(queryLabelStations, 4,0,1,2);
    layout->addWidget(resultViewStations, 5,0,1,2);
    //toolbox->addItem(page, tr("Game"));

    fileStations.close();
}
