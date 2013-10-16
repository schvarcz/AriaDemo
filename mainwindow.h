#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <aboutdialog.h>
#include <robot.h>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void dialChanged(int value);
    void openAboutDialog();
    void upClicked();
    void downClicked();
    void leftClicked();
    void rightClicked();
    void updateData();
    void startStopRobot();

signals:
    void moving(int distanceMM);
    
private:
    void connectActions();
    Ui::MainWindow *ui;
    AboutDialog aboutDialog;
    Robot *mRobot;
    QTimer mTimer;
};

#endif // MAINWINDOW_H
