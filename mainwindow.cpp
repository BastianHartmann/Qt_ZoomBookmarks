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
    currMeeting.setValue("Meeting1");

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
        // Make the button a child of the verticalLayout_ScrollArea
        ui->verticalLayout_ScrollArea->addWidget(newButton);
        // Define the objectName of the button for later identification
        newButton->setObjectName(buttonName);
        // Set the size plicy to 'Minimum' (horizontally) and 'Fixed' (vertically)
        newButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
        // Set the minimum size to 0 (horizontally) and 60 (vertically)
        newButton->setMinimumSize(0,60);
        // Set the maximum size to ~inf (horizontally) and 60 (vertically)
        newButton->setMaximumSize(16777215,60);
        // Define font of button
        QFont buttonFont("Segoe UI",9);
        buttonFont.setBold(false);
        buttonFont.setStrikeOut(false);
        buttonFont.setKerning(true);
        // Set button font
        newButton->setFont(buttonFont);
        // Set auto fill background to false
        newButton->setAutoFillBackground(false);
        // Align the button text left via a style sheet
        newButton->setStyleSheet("text-align:left");
        // Set the text of the button
        QString button_text = name + "\n" + date + "\n" + startTime + " - " + endTime;
        newButton->setText(button_text);
        // Set the button icon
        newButton->setIcon(QIcon(":/Meeting_icon_zoom_bookmarks.png"));
        // Set the size of the icon to 60 x 60
        newButton->setIconSize(QSize(60,60));
        // Set the checkable property to true
        newButton->setCheckable(true);
        // Set the checked and default state to false - at startup, only the first meeting button is checked.
        newButton->setChecked(false);
        newButton->setDefault(false);
        newButton->setFlat(false);
        // connect the new button to the clicked() slot of the meeting1 button.
        connect(newButton,SIGNAL(clicked()),this,SLOT(on_Meeting1_clicked()));

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


void MainWindow::on_Meeting1_clicked()
{
    QObject* sendObj = sender();
    QString sendName = sendObj->objectName();
    QString oldName = currMeeting.value();
    if(!(sendName==oldName)){
        currMeeting.setValue(sendName);
        QList<QPushButton*> buttonList = ui->scrollAreaWidgetContents->findChildren<QPushButton*>();
        int listLength = buttonList.size();
        for(int i = 0; i < listLength; ++i){
            QPushButton* currBut = buttonList[i];
            QString currName = currBut->objectName();
            if (!(currName == sendName)){
                currBut->setChecked(false);
            }
        }
        // Set InfoBox text for selected meeting
        QJsonObject jObj = jsonObj.value();
        QJsonValue meetJsonVal = jObj.value(sendName);
        QString infoMeetText = FormatInfoText(meetJsonVal);
        ui->InformationDisplay->clear();
        ui->InformationDisplay->setHtml(infoMeetText);
    }
    else {
        QPushButton* currBut = ui->scrollAreaWidgetContents->findChild<QPushButton*>(sendName);
        currBut->setChecked(true);
    }

    qDebug() << "A button was clicked: " << sendName << Qt::endl;

}

