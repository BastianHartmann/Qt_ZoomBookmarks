#ifndef ADDMEETINGDIALOG_H
#define ADDMEETINGDIALOG_H

#include <QDialog>

namespace Ui {
class AddMeetingDialog;
}

class AddMeetingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMeetingDialog(QWidget *parent = nullptr);
    ~AddMeetingDialog();
    QProperty<QString>selMeetingType;

private slots:
    void on_radioButton_typeRecurr_clicked();

    void on_radioButton_typeSingle_clicked();

    void on_buttonBox_Save_Cancel_accepted();

private:
    Ui::AddMeetingDialog *ui;
};

#endif // ADDMEETINGDIALOG_H
