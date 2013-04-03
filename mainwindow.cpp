#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    aboutDialog(this),
    mTimer(this)
{
    ui->setupUi(this);

    //Just to simulate the command line input
    int argc = 0;
    char *argv;
    mRobot = new Robot(&argc,&argv);

    mTimer.setInterval(1000);
    mTimer.start();

    this->connectActions();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dialChanged(int value)
{
    //Just to lock within 180º (the range of robot)
    if( value > 270)
        ui->dialAngle->setValue(270);
    else if(value < 90)
        ui->dialAngle->setValue(90);

}

void MainWindow::openAboutDialog()
{
    aboutDialog.show();
}

void MainWindow::upClicked()
{
    mRobot->move(100);
}

void MainWindow::downClicked()
{
    mRobot->move(-100);
}

void MainWindow::rightClicked()
{
    mRobot->rotate(-10);
}

void MainWindow::leftClicked()
{
    mRobot->rotate(10);
}

void MainWindow::updateData()
{
    mRobot->readingSensors();

    ui->LEAngle->setText(QString::number(ui->dialAngle->value()-90));
    ui->LEValue->setText(QString::number(mRobot->getLaserRange(ui->dialAngle->value()-90)));

    ui->LEXPosition->setText(QString::number(mRobot->getX()));
    ui->LEYPosition->setText(QString::number(mRobot->getY()));
    ui->LEHeading->setText(QString::number(mRobot->getNorth()));
}

void MainWindow::connectActions()
{
    this->connect(&mTimer, SIGNAL(timeout()),this,SLOT(updateData()));

    this->connect(ui->dialAngle,SIGNAL(sliderMoved(int)),this,SLOT(dialChanged(int)));
    this->connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(openAboutDialog()));

    this->connect(ui->actionUP,SIGNAL(triggered()),this,SLOT(upClicked()));
    this->connect(ui->actionDOWN,SIGNAL(triggered()),this,SLOT(downClicked()));
    this->connect(ui->actionLEFT,SIGNAL(triggered()),this,SLOT(leftClicked()));
    this->connect(ui->actionRIGHT,SIGNAL(triggered()),this,SLOT(rightClicked()));

    this->connect(ui->PBUp,SIGNAL(clicked()),this,SLOT(upClicked()));
    this->connect(ui->PBDown,SIGNAL(clicked()),this,SLOT(downClicked()));
    this->connect(ui->PBLeft,SIGNAL(clicked()),this,SLOT(leftClicked()));
    this->connect(ui->PBRight,SIGNAL(clicked()),this,SLOT(rightClicked()));


}
