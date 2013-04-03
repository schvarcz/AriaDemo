#ifndef ROBOT_H
#define ROBOT_H

#include <Aria.h>
#include <iostream>

using namespace std;

class Robot
{
public:
    Robot(int *argc, char **argv);
    ~Robot();
    void run();
    void move(int distanceMM);
    void rotate(int degrees);
    int getLaserRange(int angle);
    int getSonarRange(int id_sonar);
    double getX();
    double getY();
    double getNorth();
    void readingSensors();
private:
    ArRobot robot;
    ArSick sick;
    ArRobotConnector robotConnector;
    ArLaserConnector laserConnector;
    ArArgumentParser parser;
    vector<ArSensorReading> *lasers;
};

#endif // ROBOT_H
