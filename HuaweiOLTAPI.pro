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
    ../QTelnet/QTelnet.cpp \
    QIniFile.cpp \
    GlobalConfig.cpp \
    QTelnetInterface.cpp \
    OLTConstants.cpp \
    QOLTCommands.cpp \
    QInfoDialog.cpp \
    OLTCommands_UnmanagedOLTs.cpp \
    OLTCommands_BoardInfo.cpp \
    OLTCommands_BaseInfo.cpp

HEADERS  += QFrontend.h \
    ../QTelnet/QTelnet.h \
    QIniFile.h \
    GlobalConfig.h \
    QTelnetInterface.h \
    OLTConstants.h \
    QOLTCommands.h \
    QInfoDialog.h \
    OLTCommands_UnmanagedOLTs.h \
    OLTCommands_BoardInfo.h \
    OLTCommands_BaseInfo.h

FORMS    += QFrontend.ui \
    QInfoDialog.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    BoardInfo0.txt \
    GetUnmanaged0.txt
