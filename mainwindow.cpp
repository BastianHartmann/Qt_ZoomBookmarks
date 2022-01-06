#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QFile jsonFile("meetings.json");

//    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
//        jsonString.setValue(jsonFile.readAll());
//    } else {
//        qDebug() << "meetings.json file was not found!" << "\n";
//    }
//    qDebug() << jsonString.value();
//    QJsonDocument meetingsDoc = QJsonDocument::fromJson(jsonString.value().toUtf8());
//    QJsonObject meetingsObj = meetingsDoc.object();
//    qDebug() << meetingsObj.value("Meeting1");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_AddButton_clicked()
{

    // QObject* parent_object = ui->Meeting1->parent();
    QList<QObject*> QObjectList = ui -> scrollAreaWidgetContents -> children();
    QObject* top_meeting = QObjectList.takeLast();
    qDebug() << top_meeting->objectName();

    QFile jsonFile("meetings.json");

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        jsonString.setValue(jsonFile.readAll());
    } else {
        qDebug() << "meetings.json file was not found!" << "\n";
    }
    qDebug() << jsonString.value();

}

