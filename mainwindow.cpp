#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QBoxLayout>
#include <QApplication>

#include <QTimer>

void MainWindow::display()
{
    QLabel *queryLabelStations = new QLabel(QApplication::translate("Policia", "Stations"));
    QFile::copy(":/data/minon-02.csv", "/home/ivo/cambada/robocop/trunk/data/temp.csv");
    QFile fileStations("/home/ivo/cambada/robocop/trunk/data/temp.csv");

    QTextStream in(&fileStations);
    QString line;
    QVBoxLayout *table = new QVBoxLayout();
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


    table->addWidget(queryLabelStations);
    table->addWidget(resultViewStations);
    qDebug() << "before"<<endl;

    setCentralWidget(resultViewStations);
    qDebug() << "after"<<endl;
    fileStations.close();
    if ( !fileStations.remove())
    {
        qDebug() << "falhou a apagar";
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    qDebug()<<"mainwindowstart"<<endl;

    timer = new QTimer(this);
    /*timer->start(1000);
        connect(timer, &QTimer::timeout, [=](){
            if(i<20){
                QList<QStandardItem *> newstandardItemsList;

                newstandardItemsList.append(new QStandardItem());
                csvModelStations->insertRow(csvModelStations->rowCount(), newstandardItemsList);
                setCentralWidget(resultViewStations);

            }
            else{
                timer->stop();
                qDebug()<<"finish";
            }
        });
        qDebug()<<"after timer"<<endl;

    connect(&timer, QTimer::timeout, this, MainWindow::display);

    */
    qDebug()<<"mainwindow timer 1"<<endl;
    timer->setInterval(750);
timer->start(1000);
    qDebug()<<"mainwindow timer 2"<<endl;
    connect(timer, &QTimer::timeout, this, &MainWindow::display);
    qDebug()<<"mainwindow timer 3"<<endl;
}
