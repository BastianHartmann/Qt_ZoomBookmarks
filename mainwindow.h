#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProperty>
#include <QString>
#include <QJsonObject>
#include <QPoint>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QProperty<QString> jsonString;

    QProperty<QString> currMeeting;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void AddMeetingButton(int meetingNum, QJsonValue jVal);

    QString FormatInfoText(QJsonValue jVal);

    QProperty<QJsonObject> jsonObj;



private slots:
    void on_AddButton_clicked();

    void on_Meeting1_clicked();

    void highlightMeetings();

    void on_TrashButton_clicked();

    void on_AttendButton_clicked();

    void on_actionAbout_ZoomBookmarks_triggered();

    void slotCustomMenuRequested(QPoint);

    void slotEditMeeting();

public slots:
    void receiveMeetingData(QJsonObject);

    void editMeetingData(QJsonObject,QString);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
