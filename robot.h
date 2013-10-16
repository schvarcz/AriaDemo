#ifndef ROBOT_H
#define ROBOT_H

#include <Aria.h>
#include <iostream>
#include <QObject>
#include <QThread>
#include <QTimer>

using namespace std;

class Robot: public QObject, public ArRobot
{
    Q_OBJECT

public:
    Robot(int *argc, char **argv);
    ~Robot();
    void rotate(int degrees);
    int getLaserRange(int angle);
    int getSonarRange(int id_sonar);
    double getNorth();
    void start();
private slots:
    bool initializeAria();
public slots:
    bool shutdown();
    void readingSensors();
    void move(int distanceMM);
private:
    ArSick sick;
    ArSonarDevice sonar;
    ArRobotConnector *robotConnector;
    ArLaserConnector *laserConnector;
    ArArgumentParser parser;
    vector<ArSensorReading> *lasers = NULL;
    int sonars[8] = {0,0,0,0,0,0,0,0};
    QThread *thread;
    QTimer *timer;
};

#endif // ROBOT_H
