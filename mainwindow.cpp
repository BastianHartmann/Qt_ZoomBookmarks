#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QFileInfo>
#include <QPushButton>
#include <QIcon>
#include <QStyleOptionButton>
#include "addmeetingdialog.h"
#include <QDate>
#include <QMessageBox>
#include <QStringList>
#include <QDesktopServices>
#include <QUrl>
#include <QPoint>
#include <QMenu>
#include <QAction>

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

    // Create PushButton Context Menu via a slot
    ui->Meeting1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->Meeting1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenuRequested(QPoint)));

    if (obj.size() > 0){
        QStringList keys = obj.keys();
        // Determine number of stored meetings
        int numOfMeetings = keys.size();
        for(int i = 0; i < numOfMeetings; ++i){
            QJsonValue curr_val = obj.value(keys.at(i));
            AddMeetingButton(i+1,curr_val);
        }
        currMeeting.setValue("Meeting1");
    }

    highlightMeetings();


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

    // Open AddMeetingDialog
    AddMeetingDialog MeetDiag(this,"add");
    MeetDiag.exec();

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
        currMeeting.setValue("Meeting1");
        // Set Text of corresponding button
        QString button_text = name + "\n" + date + "\n" + startTime + " - " + endTime;
        ui->Meeting1->setText(button_text);

        // Set Text in the InformationDisplay
        QString InfoText = FormatInfoText(jVal);
        ui->InformationDisplay->clear();
        ui->InformationDisplay->setHtml(InfoText);

        // Enable Attend Meeting button
        ui->AttendButton->setEnabled(true);

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
        // Connect to context menu signals and slots
        newButton->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(newButton,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenuRequested(QPoint)));


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
        if(!(jObj.isEmpty())){
            QJsonValue meetJsonVal = jObj.value(sendName);
            QString infoMeetText = FormatInfoText(meetJsonVal);
            ui->InformationDisplay->clear();
            ui->InformationDisplay->setHtml(infoMeetText);
        }
    }
    else {
        QPushButton* currBut = ui->scrollAreaWidgetContents->findChild<QPushButton*>(sendName);
        currBut->setChecked(true);
    }

    qDebug() << "A button was clicked: " << sendName << Qt::endl;

}

void MainWindow::receiveMeetingData(QJsonObject meetingObj)
{
    // Create new QJsonValue from meetingObj
    QJsonValue newMeetVal(meetingObj);
    // Get the stored jsonObj
    QJsonObject allMeetObj = jsonObj.value();
    // Determine number of newly added meeting
    QList<QPushButton*> buttonList = ui->scrollAreaWidgetContents->findChildren<QPushButton*>();
    int numMeetings = buttonList.size();
    // Add the button for the new meeting
    QString newName;
    if(allMeetObj.isEmpty()){
        AddMeetingButton(1,newMeetVal);
        newName = "Meeting1";
    }
    else{
        AddMeetingButton(numMeetings+1,newMeetVal);
        newName = "Meeting" + QString::number(numMeetings+1);
    }
    // Add the new meeting to jsonObj
    allMeetObj.insert(newName,newMeetVal);
    // store newObj in jsonObj property
    jsonObj.setValue(allMeetObj);

    // Convert the new QJsonObject to QJsonDocument for writing to meetings.json
    QJsonDocument writeDoc(allMeetObj);
    QFile writeFile("meetings.json");
    if(writeFile.open(QFile::WriteOnly | QFile::Text)){
        writeFile.write(writeDoc.toJson());
    }
    else {
        qDebug() << "meetings.json was not accessible for writing!" << Qt::endl;
    }
    highlightMeetings();

}

void MainWindow::highlightMeetings()
{
    QList<QPushButton*> buttonList = ui->scrollAreaWidgetContents->findChildren<QPushButton*>();
    QJsonObject jObj = jsonObj.value();
    int numMeetings = buttonList.size();
    for(int i = 0; i < numMeetings; ++i){
        QPushButton* currButton = buttonList[i];
        QString currName = currButton->objectName();
        QJsonValue currValue = jObj.value(currName);
        QString currType = currValue["Type"].toString();
        if (currType=="single"){
            QString currDateString = currValue["Date"].toString();
            QDate currDate = QDate::fromString(currDateString,"dd.MM.yyyy");
            if(currDate == QDate::currentDate()){
                currButton->setAutoFillBackground(true);
                currButton->setStyleSheet("text-align:left; background-color:green");
                currButton->setFlat(false);
            }
            else if(currDate < QDate::currentDate()){
                currButton->setStyleSheet("text-align:left; background-color:red");
            }
            else {
                currButton->setStyleSheet("text-align:left");
            }
        }
        else if (currType=="recurring"){
            QString MDateString = currValue["Date"].toString();
            QStringList DateList = MDateString.split(" - ");
            QString MDay = DateList[0];
            QString dayOfWeek = QDate::currentDate().toString("dddd");
            if(MDay==dayOfWeek){
                currButton->setStyleSheet("text-align:left; background-color:green");
            }
            else{
                currButton->setStyleSheet("text-align:left");
            }

        }
    }
}


void MainWindow::on_TrashButton_clicked()
{
    QString selection = currMeeting.value();
    QJsonObject meetingsObj = jsonObj.value();
    if(meetingsObj.isEmpty()){
        return;
    }
    QJsonValue selMeeting = meetingsObj.value(selection);
    QString selMeetingName = selMeeting["Name"].toString();
    QString questString = "Are you sure you want to delete " + selMeetingName + "?";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Deletion request",questString,QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        // Find and delete the corresponding pushButton
        selection.replace("Meeting","");
        int meetingIdx = selection.toInt()-1;
        QList<QPushButton*> buttonList = ui->scrollAreaWidgetContents->findChildren<QPushButton*>();
        QPushButton* selButton = buttonList[meetingIdx];
        if(buttonList.size()>1){
            ui->verticalLayout_ScrollArea->removeWidget(selButton);
            delete selButton;
            // Deleting the corresponding value in the QJsonObject
            QJsonObject newJsonObject;
            QStringList oldKeys = meetingsObj.keys();
            int numKeys = oldKeys.size();
            int currMeetNum = 0;
            for(int i = 0; i < numKeys; ++i){
                QString currMeetString = oldKeys[i];
                QString compName = "Meeting"+selection;
                if(currMeetString!=compName){
                    QJsonValue currVal = meetingsObj.value(currMeetString);
                    currMeetNum += 1;
                    QString newMeetingKey = "Meeting" + QString::number(currMeetNum);
                    newJsonObject.insert(newMeetingKey,currVal);
                    buttonList[i]->setObjectName(newMeetingKey);
                }
            }
            // assign the new JsonObject to the jsonObj property of MainWindow
            jsonObj.setValue(newJsonObject);
            // Write the newJsonObject to the meetings.json file
            QJsonDocument writeDoc(newJsonObject);
            QFile writeFile("meetings.json");
            if(writeFile.open(QFile::WriteOnly | QFile::Text)){
                writeFile.write(writeDoc.toJson());
            }
            else {
                qDebug() << "meetings.json was not accessible for writing!" << Qt::endl;
            }
            // Set the first button as checked
            buttonList = ui->scrollAreaWidgetContents->findChildren<QPushButton*>();
            buttonList.first()->setChecked(true);
            QJsonValue meetJsonVal = newJsonObject.value("Meeting1");
            QString infoMeetText = FormatInfoText(meetJsonVal);
            ui->InformationDisplay->clear();
            ui->InformationDisplay->setHtml(infoMeetText);
            currMeeting.setValue("Meeting1");
        }
        else{
            selButton->setStyleSheet("text-align:left");
            ui->InformationDisplay->clear();
            ui->InformationDisplay->setHtml("No meeting added");
            selButton->setText("No meeting added");
            ui->AttendButton->setEnabled(false);
            QJsonObject newJsonObject;
            QJsonDocument writeDoc(newJsonObject);
            QFile writeFile("meetings.json");
            if(writeFile.open(QFile::WriteOnly | QFile::Text)){
                writeFile.write(writeDoc.toJson());
            }
            else {
                qDebug() << "meetings.json was not accessible for writing!" << Qt::endl;
            }
            // assign the new JsonObject to the jsonObj property of MainWindow
            jsonObj.setValue(newJsonObject);
        }
    }

}


void MainWindow::on_AttendButton_clicked()
{
    QString currMName = currMeeting.value();
    QJsonValue currVal = jsonObj.value().value(currMName);
    QString MLink = currVal["MLink"].toString();
    if(!(MLink.isEmpty())){
        QDesktopServices::openUrl(QUrl(MLink));
    }

}


void MainWindow::on_actionAbout_ZoomBookmarks_triggered()
{
    QMessageBox::information(this,"About ZoomBookmarks","ZoomBoomarks - Manage your online meetings.\nVersion 1.1\nCopyright (C) 2022  Bastian Hartmann\n\nYou should have received a copy of the\nGNU General Public License along with this program.\nIf not, see: https://www.gnu.org/licenses/",QMessageBox::Ok);
}

void MainWindow::editMeetingData(QJsonObject editObj, QString editName){
    // Create new QJsonValue from meetingObj
    QJsonValue newMeetVal(editObj);
    // Get the stored jsonObj
    QJsonObject allMeetObj = jsonObj.value();
    // Add the new meeting to jsonObj
    allMeetObj[editName] = newMeetVal;
    // store newObj in jsonObj property
    jsonObj.setValue(allMeetObj);

    // Convert the new QJsonObject to QJsonDocument for writing to meetings.json
    QJsonDocument writeDoc(allMeetObj);
    QFile writeFile("meetings.json");
    if(writeFile.open(QFile::WriteOnly | QFile::Text)){
        writeFile.write(writeDoc.toJson());
    }
    else {
        qDebug() << "meetings.json was not accessible for writing!" << Qt::endl;
    }

    // Get values
    QString name = newMeetVal["Name"].toString();
    QString date = newMeetVal["Date"].toString();
    QString startTime = newMeetVal["StartTime"].toString();
    QString endTime = newMeetVal["EndTime"].toString();

    // get pointer to the edited button
    QPushButton* editedButton = ui->scrollAreaWidgetContents->findChild<QPushButton*>(editName);

    // Set Text of corresponding button
    QString button_text = name + "\n" + date + "\n" + startTime + " - " + endTime;
    editedButton->setText(button_text);

    highlightMeetings();

    // select the edited button and deselect others
    QList<QPushButton*> buttonList = ui->scrollAreaWidgetContents->findChildren<QPushButton*>();
    int length = buttonList.size();
    for(int i = 0; i < length; ++i){
        QPushButton* currButton = buttonList.at(i);
        currButton->setChecked(false);
    }

    editedButton->setChecked(true);
    // show new text in info box
    QString showText = FormatInfoText(newMeetVal);
    ui->InformationDisplay->clear();
    ui->InformationDisplay->setHtml(showText);
}

void MainWindow::slotCustomMenuRequested(QPoint pos){
    QJsonObject meetingsObj = jsonObj.value();
    if(meetingsObj.isEmpty()){
        return;
    }
    QObject* sendObj = sender();
    QPushButton* sendButton = qobject_cast<QPushButton*>(sendObj);
    QString buttonName = sendButton->objectName();
    QMenu* buttonMenu = new QMenu(sendButton);
    QAction* editMeeting = new QAction("Edit",sendButton);
    connect(editMeeting,SIGNAL(triggered()),this,SLOT(slotEditMeeting()));
    buttonMenu->addAction(editMeeting);
    buttonMenu->popup(sendButton->mapToGlobal(pos));
}

void MainWindow::slotEditMeeting(){
    // Open AddMeetingDialog in edit mode
    QObject* sendObj = sender();
    QObject* sendParent = sendObj->parent();
    QString sendMeeting = sendParent->objectName();
    AddMeetingDialog MeetDiag(this,"edit",sendMeeting);
    MeetDiag.exec();
}

