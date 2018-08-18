QT += core network
QT -= gui

CONFIG += c++11

TARGET = shkmQtFtp
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    qtshkmftpClient.cpp

HEADERS += \
    qtftpClientclass.h
