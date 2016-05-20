#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T16:04:11
#
#-------------------------------------------------

QT       += core gui
CONFIG += console
QT += core websockets
QT += sql
QT += webkit webkitwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nhl-server
TEMPLATE = app


SOURCES += main.cpp\
        serverwindow.cpp \
    server.cpp \
    databasemanager.cpp

HEADERS  += serverwindow.h \
    server.h \
    databasemanager.h

FORMS    += serverwindow.ui
