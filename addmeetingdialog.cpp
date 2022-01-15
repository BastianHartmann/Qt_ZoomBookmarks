#include "addmeetingdialog.h"
#include "ui_addmeetingdialog.h"
#include "mainwindow.h"
#include <QDialog>
#include <QJsonValue>
#include <QJsonObject>
#include <QDate>

AddMeetingDialog::AddMeetingDialog(QWidget *parent, QString addType, QString meetName) :
    QDialog(parent),
    ui(new Ui::AddMeetingDialog)
{
    ui->setupUi(this);
    guiType.setValue(addType);
    if(addType == "add"){
        selMeetingType.setValue("single");
        ui->selectDateEdit->setDate(QDate::currentDate());
        connect(this,SIGNAL(sendMeetingInfo(QJsonObject)),parent,SLOT(receiveMeetingData(QJsonObject)));
    }
    else if(addType == "edit"){
        editMeeting.setValue(meetName);
        MainWindow* parentUi = qobject_cast<MainWindow*>(parent);
        QJsonValue constructVal = parentUi->jsonObj.value().value(meetName);
        QString MName = constructVal["Name"].toString();
        QString MType = constructVal["Type"].toString();
        QString MDate = constructVal["Date"].toString();
        QString MSTime = constructVal["StartTime"].toString();
        QString METime = constructVal["EndTime"].toString();
        QString MLink = constructVal["MLink"].toString();
        QString MID = constructVal["MID"].toString();
        QString MPW = constructVal["MPW"].toString();
        selMeetingType.setValue(MType);
        if(MType=="recurring"){
            ui->radioButton_typeRecurr->setChecked(true);
            QStringList dateString = MDate.split(" - ");
            ui->selectDayDropdown->setCurrentText(dateString[0]);
            ui->selectFrequDropdown->setCurrentText(dateString[1]);
            ui->SingleDateBox->setEnabled(false);
            ui->RecurrOptionBox->setEnabled(true);
            ui->selectDateEdit->setDate(QDate::currentDate());
        }
        else {
            QDate mDate = QDate::fromString(MDate,"dd.MM.yyyy");
            ui->selectDateEdit->setDate(mDate);
        }
        ui->MeetingName_lineEdit->setText(MName);
        ui->StartTimeEdit->setTime(QTime::fromString(MSTime,"hh:mm"));
        ui->EndTimeEdit->setTime(QTime::fromString(METime,"hh:mm"));
        ui->linkLineEdit->setText(MLink);
        ui->IDLineEdit->setText(MID);
        ui->PWLineEdit->setText(MPW);
        connect(this,SIGNAL(sendMeetingInfo(QJsonObject,QString)),parent,SLOT(editMeetingData(QJsonObject,QString)));
    }
}

AddMeetingDialog::~AddMeetingDialog()
{
    delete ui;
}

void AddMeetingDialog::on_radioButton_typeRecurr_clicked()
{
    ui->SingleDateBox->setEnabled(false);
    ui->RecurrOptionBox->setEnabled(true);
    selMeetingType.setValue("recurring");
}


void AddMeetingDialog::on_radioButton_typeSingle_clicked()
{
    ui->RecurrOptionBox->setEnabled(false);
    ui->SingleDateBox->setEnabled(true);
    selMeetingType.setValue("single");
}


void AddMeetingDialog::on_buttonBox_Save_Cancel_accepted()
{
    QString selType = selMeetingType.value();

    // Declare all variables
    QString MName;
    QString MType;
    QString MDate;
    QString MStart;
    QString MEnd;
    QString MLink;
    QString MID;
    QString MPW;

    if(selType=="single"){
        // single meeting handling
        MName = ui->MeetingName_lineEdit->text();
        MType = selType;
        MDate = ui->selectDateEdit->date().toString("dd.MM.yyyy");
        MStart = ui->StartTimeEdit->text();
        MEnd = ui->EndTimeEdit->text();
        MLink = ui->linkLineEdit->text();
        MID = ui->IDLineEdit->text();
        MPW = ui->PWLineEdit->text();

    }
    else if(selType=="recurring"){
        // recurring meeting handling
        MName = ui->MeetingName_lineEdit->text();
        MType = selType;
        QString MDay = ui->selectDayDropdown->currentText();
        QString MFrequ = ui->selectFrequDropdown->currentText();
        MDate = MDay + " - " + MFrequ;
        MStart = ui->StartTimeEdit->text();
        MEnd = ui->EndTimeEdit->text();
        MLink = ui->linkLineEdit->text();
        MID = ui->IDLineEdit->text();
        MPW = ui->PWLineEdit->text();
        
        qDebug() << "Generate recurring meeting." << Qt::endl;
    }

    // Create QJsonObject
    QJsonObject jsonMeetObj;
    jsonMeetObj.insert("Name",MName);
    jsonMeetObj.insert("Type",MType);
    jsonMeetObj.insert("Date",MDate);
    jsonMeetObj.insert("StartTime",MStart);
    jsonMeetObj.insert("EndTime",MEnd);
    jsonMeetObj.insert("MLink",MLink);
    jsonMeetObj.insert("MID",MID);
    jsonMeetObj.insert("MPW",MPW);

    // send QString and QJsonObject to MainWindow
    if(guiType.value()=="add"){
        emit sendMeetingInfo(jsonMeetObj);
    }
    else{
        emit sendMeetingInfo(jsonMeetObj,editMeeting.value());
    }
}

