#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDir>
#include <QFileInfo>
#include <QPushButton>
#include <QIcon>
#include <QStyleOptionButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Creating the QFile object to the meetings.json file.
    QFile jsonFile("meetings.json");

    // Open the meetings.json QFile.
    if (jsonFile.open(QFile::ReadOnly | QFile::Text)){
        jsonString.setValue(jsonFile.readAll());}
    else{
        qDebug() << "meetings.json file was not found!" << "\n";
    }

    // Convert the jsonString to a QJsonObject and store it in the jsonObj property.
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.value().toUtf8());
    jsonObj.setValue(jsonDoc.object());
    QJsonObject obj = jsonObj.value();

    if (obj.size() > 0){
            QStringList keys = obj.keys();
            // Determine number of stored meetings
            QString lastMeeting = keys.last();
            lastMeeting.replace("Meeting","");
            int numOfMeetings = lastMeeting.toInt();
            for(int i = 0; i < numOfMeetings; ++i){
                QJsonValue curr_val = obj.value(keys.at(i));
                AddMeetingButton(i+1,curr_val);
            }
        }



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

}

void MainWindow::AddMeetingButton(int meetingNum, QJsonValue jVal)
{
    QString buttonName = "Meeting" + QString::number(meetingNum);

    // Get Infos from Json value for the meeting button
    QString name = jVal["Name"].toString();
    QString date = jVal["Date"].toString();
    QString startTime = jVal["StartTime"].toString();
    QString endTime = jVal["EndTime"].toString();

    if(meetingNum==1)
    {
        // Set Text of corresponding button
        QString button_text = name + "\n" + date + "\n" + startTime + " - " + endTime;
        ui->Meeting1->setText(button_text);

        // Set Text in the InformationDisplay
        QString InfoText = FormatInfoText(jVal);
        ui->InformationDisplay->clear();
        ui->InformationDisplay->setHtml(InfoText);

    }
    else
    {
        // Code for other Meetings
        QPushButton* newButton = new QPushButton(this);
        ui->verticalLayout_ScrollArea->addWidget(newButton);
        newButton->objectName() = buttonName;
    }

}

QString MainWindow::FormatInfoText(QJsonValue jVal)
{
    // Get Infos from Json value
    QString name = jVal["Name"].toString();
    QString date = jVal["Date"].toString();
    QString startTime = jVal["StartTime"].toString();
    QString endTime = jVal["EndTime"].toString();
    QString link = jVal["MLink"].toString();
    QString ID = jVal["MID"].toString();
    QString Pw = jVal["MPW"].toString();

    QString outputText = "<b>Meeting:</b><br>" + name + "<br><br>" + "<b>Date:</b><br>" + date + "<br><br>" + "<b>Time:</b><br>" + startTime + " - " + endTime + "<br><br><br>" + "<b><u>Zoom Info:</u></b><br><br>" + "<b>Link:</b> <a href="+link+">" + link + "</a><br><br>" + "<b>Meeting-ID:</b> " + ID + "<br>" + "<b>Meeting-PW:</b> " + Pw + "<br>";

    return outputText;
}

