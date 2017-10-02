#-------------------------------------------------
#
# Project created by QtCreator 2017-10-02T22:56:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HuaweiOLTAPI
TEMPLATE = app


SOURCES += main.cpp\
        QFrontend.cpp \
    ../QTelnet/QTelnet.cpp

HEADERS  += QFrontend.h \
    ../QTelnet/QTelnet.h

FORMS    += QFrontend.ui
