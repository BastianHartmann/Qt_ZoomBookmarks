#include "addmeetingdialog.h"
#include "ui_addmeetingdialog.h"

AddMeetingDialog::AddMeetingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMeetingDialog)
{
    ui->setupUi(this);
    selMeetingType = "single";
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
    if(selType=="single"){
        // single meeting handling
        // ENTER CODE HERE
        qDebug() << "Generate single meeting." << Qt::endl;
    }
    else if(selType=="recurring"){
        // recurring meeting handling
        // ENTER CODE HERE
        qDebug() << "Generate recurring meeting." << Qt::endl;
    }
}

