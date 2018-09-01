#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T23:54:39
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia
QT       += multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-------------------------------------------------
#配置winpacp
INCLUDEPATH += WpdPack\Include
LIBS += -L../ls/  \
        -lPacket  \
        -lwpcap  \
        -lWS2_32  \
        -lwlan
#-------------------------------------------------
#编译预处理文件 可以增加编译速度
PRECOMPILED_HEADER = stable.h
#-------------------------------------------------
TARGET = ls
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    selectip.cpp \
    server.cpp \
    client.cpp \
    tcpsocket.cpp \
    tcpserver.cpp \
    udpbroad.cpp \
    verification.cpp \
    searchwifi.cpp \
    headrectitem.cpp \
    instruction.cpp \
    cameraview.cpp \
    httpserver.cpp \
    httpsocket.cpp

    instruction.cpp

    headrectitem.cpp


HEADERS += \
        mainwindow.h \
    stable.h \
    selectip.h \
    server.h \
    client.h \
    tcpsocket.h \
    tcpserver.h \
    udpbroad.h \
    verification.h \
    wlan.h \
    searchwifi.h \
    headrectitem.h \
    instruction.h \
    cameraview.h \
    httpserver.h \
    httpsocket.h

    instruction.h

    headrectitem.h


FORMS += \
        mainwindow.ui \
    arpactdev.ui \
    selectip.ui \
    server.ui \
    client.ui \
    verification.ui \
    instruction.ui \
    instruction.ui \
    cameraview.ui

OTHER_FILES += \
            Packet.lib\
            wpcap.lib\
            WS2_32.lib\
            wlan.lib
