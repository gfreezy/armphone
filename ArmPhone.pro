#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T19:18:23
#
#-------------------------------------------------

QT       += core gui network

TARGET = ArmPhone
TEMPLATE = app


SOURCES +=\
    audiodevice.cpp \
    datasocket.cpp \
    audiodatasocket.cpp \
    audiodatasocketthread.cpp \
    videodevice.cpp \
    videodatasocket.cpp \
    videodatasocketthread.cpp \
    videowidget.cpp \
    dialog.cpp \
    main.cpp

HEADERS  += \
    audiodevice.h \
    datasocket.h \
    audiodatasocket.h \
    audiodatasocketthread.h \
    videodevice.h \
    videodatasocket.h \
    videodatasocketthread.h \
    videowidget.h \
    dialog.h

#INCLUDEPATH += gsm/inc

FORMS    += \
    dialog.ui
