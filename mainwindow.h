#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProperty>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QProperty<QString> jsonString;
    QProperty<QJsonObject> jsonObj;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddMeetingButton(int meetingNum, QJsonValue jVal);
    QString FormatInfoText(QJsonValue jVal);



private slots:
    void on_AddButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
