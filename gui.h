#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QBoxLayout>
#include <QToolBox>
#include <QProcess>

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
    void processAirodump();
    QString *networkName=NULL;
    QToolBox *toolbox;
    QLabel *networkNameLabel;
    QWidget *page;
    QComboBox *comboBoxWifiChannel;
    QComboBox *comboBoxTeamA;
    QComboBox *comboBoxTeamB;
    QLabel *teamATableLabel;
    QLabel *teamBTableLabel;
    QLabel *teamNoneTableLabel;
    QTimer *timer;
    QGridLayout *layout;
    QProcess *airodump;
    QString *cap_file_name = NULL;
};

#endif // GUI_H
