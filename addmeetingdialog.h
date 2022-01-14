#ifndef ADDMEETINGDIALOG_H
#define ADDMEETINGDIALOG_H

#include <QDialog>
#include <QString>
#include <QJsonObject>

namespace Ui {
class AddMeetingDialog;
}

class AddMeetingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMeetingDialog(QWidget *parent = nullptr,QString callType = "add", QString meetName = "Meeting1");
    ~AddMeetingDialog();

    QProperty<QString>selMeetingType;

    QProperty<QString>editMeeting;

    QProperty<QString>guiType;

private slots:
    void on_radioButton_typeRecurr_clicked();

    void on_radioButton_typeSingle_clicked();

    void on_buttonBox_Save_Cancel_accepted();

signals:
    void sendMeetingInfo(QJsonObject);

    void sendMeetingInfo(QJsonObject,QString);

private:
    Ui::AddMeetingDialog *ui;
};

#endif // ADDMEETINGDIALOG_H
