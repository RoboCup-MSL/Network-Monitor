#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void display();

private:
    QTimer *timer;
    QTableWidget* resultViewStations;
};

#endif // MAINWINDOW_H
