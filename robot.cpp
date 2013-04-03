#include "robot.h"

Robot::Robot(int *argc, char **argv):
    parser(argc,argv),
    robotConnector(&parser,&robot),
    laserConnector(&parser,&robot,&robotConnector)
{
    Aria::init();
    parser.loadDefaultArguments();
    robot.addRangeDevice(&sick);
    robotConnector.parseArgs();
    if(!robotConnector.connectRobot())
    {
        ArLog::log(ArLog::Terse,"Ops... falha ao conectar ao servidor do robo.");
        Aria::exit(1);
    }
    laserConnector.setupLaser(&sick);
    if(!laserConnector.connectLaser(&sick))
    {
        ArLog::log(ArLog::Terse,"Ops... falha ao conectar os lasers do robo.");
        Aria::exit(1);
    }
    ArLog::log(ArLog::Normal,"Robot connected");
    sick.runAsync();
    robot.runAsync(true);
    ArUtil::sleep(500);
    robot.lock();
    robot.enableMotors();
    robot.unlock();
    ArUtil::sleep(500);
}

Robot::~Robot()
{
    robot.stopRunning();
    robot.waitForRunExit();

    Aria::shutdown();
}

void Robot::run()
{
    int rotate = 0;
    vector<ArSensorReading> *readings;

    while(robot.isConnected())
    {
        if(sick.isConnected())
        {
            sick.lockDevice();
            readings = sick.getRawReadingsAsVector();
            sick.unlockDevice();

            if(rotate !=0 && (readings->at(rotate+90).getRange()>= 1000))
            {
                this->rotate(-rotate);

                rotate = 0;
            }
            if(readings->at(90).getRange()>= 500)
            {
                if(rotate == 0)
                {
                    this->move(readings->at(90).getRange()-500);
                }
                else
                {
                    this->move(500);
                }
            }
            else
            {

                if(readings->at(90).getRange() < 500)
                {
                    this->move(-500+(int)readings->at(90).getRange());
                }
                if(readings->at(0).getRange()>= 800)
                {
                    rotate = 90;
                }
                else if(readings->at(180).getRange()>= 800)
                {
                    rotate = -90;
                }
                else
                {
                    rotate = 180;
                }

                this->rotate(rotate);
            }
        }
        else
        {
            cout << "sensor DESconectado..." << endl;
        }
    }
}

void Robot::readingSensors()
{
    if(robot.isConnected() && sick.isConnected())
    {
            sick.lockDevice();
            lasers = sick.getRawReadingsAsVector();
            sick.unlockDevice();
    }
}

int Robot::getLaserRange(int angle)
{
    if(this->lasers->size() <= angle)
        return -1;

    return this->lasers->at(angle).getRange();
}

double Robot::getX()
{
    return robot.getX();
}

double Robot::getY()
{
    return robot.getY();
}

double Robot::getNorth()
{
    return robot.getTh();
}

void Robot::move(int distanceMM)
{
    if(!robot.isMoveDone())
        return;
    ArLog::log(ArLog::Normal,"Movendo: %d",distanceMM);
    robot.lock();
    robot.move(distanceMM);
    robot.unlock();
    int timeout = 5000/distanceMM;
    while((!robot.isMoveDone()) && (timeout >= 0))
    {
        ArUtil::sleep(distanceMM);
        timeout--;
    }
}
void Robot::rotate(int degrees)
{
    if(!robot.isHeadingDone())
        return;
    ArLog::log(ArLog::Normal,"Rotacionando: %d",degrees);
    robot.lock();
    robot.setDeltaHeading(degrees);
    robot.unlock();
    while(!robot.isHeadingDone())
    {
        ArUtil::sleep(500);
    }
}
