#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QtGui>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget window;

    QLabel *queryLabelA = new QLabel(
            QApplication::translate("Policia", "Equipa A:"));

    QVBoxLayout *table = new QVBoxLayout();
    QStandardItemModel *csvModelA = new QStandardItemModel();
    QStandardItemModel *csvModelB = new QStandardItemModel();

    QTableView *resultViewA = new QTableView();
    resultViewA->setModel(csvModelA);
    QTableView *resultViewB = new QTableView();
    resultViewB->setModel(csvModelB);


    csvModelA->setColumnCount(2);
    csvModelA->setHorizontalHeaderLabels(QStringList() << "MAC" << "Description");

    QFile fileA(":/equipaA.csv");
    if ( !fileA.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&fileA);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QList<QStandardItem *> standardItemsList;
            foreach (QString item, line.split(";")) {
                standardItemsList.append(new QStandardItem(item));
            }
            csvModelA->insertRow(csvModelA->rowCount(), standardItemsList);
        }
        fileA.close();
    }

    QLabel *queryLabelB = new QLabel(
            QApplication::translate("Policia", "Equipa B:"));

    csvModelB->setColumnCount(2);
    csvModelB->setHorizontalHeaderLabels(QStringList() << "MAC" << "Description");

    QFile fileB(":/equipaB.csv");
    if ( !fileB.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&fileB);
        while (!in.atEnd())
        {    table->addWidget(queryLabelA);

            QString line = in.readLine();
            QList<QStandardItem *> standardItemsList;
            foreach (QString item, line.split(";")) {
                standardItemsList.append(new QStandardItem(item));
            }
            csvModelB->insertRow(csvModelB->rowCount(), standardItemsList);
        }
        fileB.close();
    }
    table->addWidget(queryLabelA);
    table->addWidget(resultViewA);
    table->addWidget(queryLabelB);
    table->addWidget(resultViewB);

    window.setLayout(table);
    window.show();
    //w.show();

    return a.exec();
}
