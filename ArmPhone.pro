#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T19:18:23
#
#-------------------------------------------------

QT       += core gui network

TARGET = ArmPhone
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audiodevice.cpp \
    netrecthread.cpp \
    udpsocket.cpp \
    netplaythread.cpp

HEADERS  += mainwindow.h \
    audiodevice.h \
    netrecthread.h \
    udpsocket.h \
    netplaythread.h

INCLUDEPATH += gsm/inc

FORMS    += mainwindow.ui

LIBS += -L gsm/lib -lgsm
