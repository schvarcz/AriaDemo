#include "robot.h"


Robot::Robot(int *argc, char **argv):
    QObject(),
    ArRobot(),
    parser(argc,argv)
{
    robotConnector = new ArRobotConnector(&parser,this);
    laserConnector = new ArLaserConnector(&parser,this,robotConnector);

    thread = new QThread();
    this->moveToThread(thread);
    connect(thread,SIGNAL(started()),this,SLOT(initializeAria()));
    connect(thread,SIGNAL(finished()),this,SLOT(shutdown()));

    timer = new QTimer();
    timer->setInterval(500);
    connect(timer,SIGNAL(timeout()),this,SLOT(readingSensors()));
}

Robot::~Robot()
{
    this->shutdown();
}

void Robot::start()
{
    thread->start();
}

bool Robot::initializeAria()
{
    Aria::init();
    //parser->addDefaultArgument("-rh 192.168.1.11 -remoteLaserTcpPort 10002");
    parser.addDefaultArgument("-rh 127.0.0.1:8101");

    this->addRangeDevice(&sick);
    if(!robotConnector->connectRobot())
    {
        ArLog::log(ArLog::Terse,"Ops... falha ao conectar ao servidor do robo.");
        this->shutdown();
        return false;
    }
    ArLog::log(ArLog::Normal,"Robot connected");
    laserConnector->setupLaser(&sick);

    if(!laserConnector->connectLaser(&sick))
    {
        ArLog::log(ArLog::Terse,"Ops... falha ao conectar os lasers do robo.");
        this->shutdown();
        return false;
    }
    list<ArRangeDevice *> *lista = this->getRangeDeviceList();
    for(list<ArRangeDevice *>::iterator it = lista->begin(); it != lista->end();it++)
    {
        cout << "Sensor: " << (*it)->getName() << endl;
    }
    ArLog::log(ArLog::Normal,"Laser connected");
    sick.runAsync();
    this->runAsync(true);
    ArUtil::sleep(500);
    this->lock();
    this->enableMotors();
    this->unlock();
    ArUtil::sleep(500);
    timer->start();

    return true;
}

bool Robot::shutdown()
{
    this->thread->exit();
    this->stopRunning();
    this->waitForRunExit();
    this->sick.stopRunning();
    this->sick.disconnect();
    ArRobot::disconnect();
    Aria::shutdown();
    return true;
}

void Robot::readingSensors()
{
    if(this->isConnected())
    {
        if(sick.isConnected())
        {
            sick.lockDevice();
            lasers = sick.getRawReadingsAsVector();
            sick.unlockDevice();
        }
        for(int i = 0;i<8;i++)
        {
            sonars[i] = ArRobot::getSonarRange(i);
        }
    }
}

int Robot::getLaserRange(int angle)
{
    if((this->lasers == NULL) || (this->lasers->size() <= angle))
    {
        return 0;
    }

    return this->lasers->at(angle).getRange();
}

int Robot::getSonarRange(int id_sonar)
{
    if(8 <= id_sonar)
    {
        return 0;
    }

    return this->sonars[id_sonar];
}

double Robot::getNorth()
{
    return this->getTh();
}

void Robot::move(int distanceMM)
{
    ArLog::log(ArLog::Normal,"Movendo: %d",distanceMM);
    this->lock();
    this->move(distanceMM);
    this->unlock();
}

void Robot::rotate(int degrees)
{
    ArLog::log(ArLog::Normal,"Rotacionando: %d",degrees);
    this->lock();
    this->setDeltaHeading(degrees);
    this->unlock();
}
