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
    datasocket.cpp \
    audiodatasocket.cpp \
    audiodatasocketthread.cpp

HEADERS  += mainwindow.h \
    audiodevice.h \
    datasocket.h \
    audiodatasocket.h \
    audiodatasocketthread.h

#INCLUDEPATH += gsm/inc

FORMS    += mainwindow.ui

LIBS += -ljthread -ljrtp
