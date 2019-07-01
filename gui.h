/** @file gui.h
 *  @brief Function prototypes for the UI
 *
 *  This contains the prototypes for the UI
 *  and the macros, constants,
 *  or global variables you will need.
 *
 *  @author Ivo Angélico
 *  @author César Gomes
 *  @bug No known bugs.
 */

#ifndef GUI_H
#define GUI_H

/* -- Includes -- */

/* QT libs includes. */
#include <QWidget>
#include <QBoxLayout>
#include <QToolBox>
#include <QProcess>
#include <QtWidgets>
#include <QPushButton>

/* Roboc0p objects includes. */
#include "player.h"

/* macros */
#define NUMBEROFCOLUMNS 7
#define NUMBEROFOTHERCOLUMNS 8
#define INTERFACELABELCOL 0
#define INTERFACELABELROW 0
#define INTERFACECOMBOBOXCOL 1
#define INTERFACECOMBOBOXROW 0
#define NETWORKSLABELCOL 0
#define NETWORKSLABELROW 1
#define NETWORKSCOMBOBOXCOL 1
#define NETWORKSCOMBOBOXROW 1
#define REFRESHBUTTONCOL 1
#define REFRESHBUTTONROW 2
#define TEAMALABELCOL 0
#define TEAMALABELROW 3
#define TEAMACOMBOBOXCOL 1
#define TEAMACOMBOBOXROW 3
#define TEAMBLABELCOL 0
#define TEAMBLABELROW 4
#define TEAMBCOMBOBOXCOL 1
#define TEAMBCOMBOBOXROW 4
#define STARTBUTTONCOL 1
#define STARTBUTTONROW 5
#define TEAMATABLELABELROW 0
#define TEAMATABLEROW 1
#define TEAMBTABLELABELROW 2
#define TEAMBTABLEROW 3
#define OTHERSTATIONSLABELROW 0
#define OTHERSTATIONSROW 3
#define WARNINGSROW 4

#define TEAMA 0
#define TEAMB 1
#define OTHERSTATIONS 2

/* global variables */
class QLabel;
class QComboBox;

/* gui class*/
class gui : public QWidget
{
    Q_OBJECT
public:
    explicit gui(QWidget *parent = 0);

signals:

public slots:
    void on_interfaceComboBox_currentIndexChanged();
    void on_comboBoxTeamA_currentIndexChanged();
    void on_comboBoxTeamB_currentIndexChanged();
    void display();
    void toggleGame();

private slots:
    void handleRefreshButton();

private:
    void refreshAndShowNetworks();
    void showInterfacesAndNetworks();
    bool proccessNetworkInformation();
    bool isOnGame();
    void startGame();
    void stopGame();
    QPushButton *startButton;
    void changeGameTeams(int comboBoxIndex, QString teamName);
    QList<QStandardItem *> addPlayerToList(player *player_to_add, bool showTeam);
    void updateTable(int tableIndex);
    void processAirodump();
    QStringList *iw_list;
    QStringList *networks_list;
    QString *networkName=NULL;
    QToolBox *toolbox;
    QLabel *networkNameLabel;
    QWidget *inputsPage;
    QWidget *gamePage;
    QWidget *otherStationsPage;
    QComboBox *comboBoxWifiChannel;
    QComboBox *comboBoxTeamA;
    QComboBox *comboBoxTeamB;
    QComboBox *interfaceComboBox;
    QComboBox *networksComboBox;

    QLabel *teamATableLabel;
    QLabel *teamBTableLabel;
    QLabel *warningLabel;

    QLabel *teamNoneTableLabel;
    QTimer *timer;
    QGridLayout *inputsLayout;
    QGridLayout *gameLayout;
    QGridLayout *otherStationsLayout;
    QProcess *airodump;
    QString *cap_file_name = NULL;
    QTableView *teamResultView[3];
    QStandardItemModel *teamAStations;
    QStandardItemModel *teamBStations;
    QStandardItemModel *otherStations;
    bool onGame = false;
};

#endif // GUI_H
