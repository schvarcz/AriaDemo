#-------------------------------------------------
#
# Project created by QtCreator 2013-04-03T16:44:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjetoDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    robot.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    robot.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

INCLUDEPATH += /usr/local/Aria/include

LIBS += -L/usr/local/Aria/lib/ \
    -lAria -lArNetworking
