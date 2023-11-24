#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T19:49:24
#
#-------------------------------------------------

QT       += core gui sql widgets

TARGET = Eserver
TEMPLATE = app


SOURCES += ESmain.cpp\
    dbkeeper.cpp \
    ESwindow.cpp \
    NetMaster.cpp \
    Brushes.cpp

HEADERS  += \
    dbkeeper.h \
    ESwindow.h \
    netmaster.h \
    Brushes.h \
    Enums.h
QT           += network
FORMS    +=
