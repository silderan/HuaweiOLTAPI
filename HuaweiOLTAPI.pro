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
    OLTCommands_BaseInfo.cpp \
    OLTCommands_ONTWANInfo.cpp \
    OLTCommands_ONTMACInfo.cpp \
    OLTCommands_GPONServiceProfile.cpp \
    OLTCommands_ONTVersion.cpp \
    OLTCommands_TrafficTableIP.cpp \
    OLTCommands_DBAProfile.cpp \
    OLTCommands_ServicePort.cpp \
    OLTCommands_LineProfile.cpp

HEADERS  += QFrontend.h \
    ../QTelnet/QTelnet.h \
    QIniFile.h \
    GlobalConfig.h \
    QTelnetInterface.h \
    QOLTCommands.h \
    QInfoDialog.h \
    OLTCommands_UnmanagedOLTs.h \
    OLTCommands_BoardInfo.h \
    OLTCommands_BaseInfo.h \
    OLTCommands_ONTWANInfo.h \
    OLTConstants.h \
    OLTCommands_ONTMACInfo.h \
    OLTCommands_GPONServiceProfile.h \
    OLTCommands_ONTVersion.h \
    OLTCommands_TrafficTableIP.h \
    OLTCommands_DBAProfile.h \
    OLTCommands_ServicePort.h \
    OLTCommands_LineProfile.h

FORMS    += QFrontend.ui \
    QInfoDialog.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    GetUnmanaged0.txt \
    AddingCommand.txt \
    AddTrafficTableIP0.txt \
    AddTrafficTableIP1.txt \
    GetDBAProfiles0.txt \
    GetBoardInfo0.txt \
    GetGPONServiceProfile0.txt \
    GetGPONServiceProfiles0.txt \
    GetONTMACInfo0.txt \
    GetONTVersion0.txt \
    GetONTWANInfo0.txt \
    GetTrafficTableIP0.txt \
    GetTrafficTableIPs0.txt \
    GetServicePorts0.txt \
    GetServicePort0.txt \
    GetONTServicePort0.txt \
    GetGPONLineProfile0.txt \
    GetGPONLineProfiles0.txt \
    GetGPONLineProfile1.txt \
    GetDBAProfile0.txt
