#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QBoxLayout>
#include <QToolBox>
#include <QProcess>
#include <QtWidgets>
#include "player.h"

#define NUMBEROFWIFICHANNELS 41
class QLabel;
class QComboBox;

class gui : public QWidget
{
    Q_OBJECT
public:
    explicit gui(QWidget *parent = 0);

signals:

public slots:
    void setNetworkName();
    void on_comboBoxWifiChannel_currentIndexChanged();
    void on_comboBoxTeamA_currentIndexChanged();
    void on_comboBoxTeamB_currentIndexChanged();
    void display();
private slots:

private:
    void changeGameTeams(int comboBoxIndex, QString teamName);
    QList<QStandardItem *> addPlayerToList(player *player_to_add);
    void updateTable(QStandardItemModel *stations, int tableIndex);
    void processAirodump();
    QString *networkName=NULL;
    QToolBox *toolbox;
    QLabel *networkNameLabel;
    QWidget *inputsPage;
    QWidget *gamePage;
    QComboBox *comboBoxWifiChannel;
    QComboBox *comboBoxTeamA;
    QComboBox *comboBoxTeamB;
    QLabel *teamATableLabel;
    QLabel *teamBTableLabel;
    QLabel *teamNoneTableLabel;
    QTimer *timer;
    QGridLayout *inputsLayout;
    QGridLayout *gameLayout;
    QProcess *airodump;
    QString *cap_file_name = NULL;
    QTableView *teamResultView[3];
};

#endif // GUI_H
