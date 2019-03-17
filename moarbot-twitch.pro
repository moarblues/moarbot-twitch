#-------------------------------------------------
#
# Project created by QtCreator 2019-02-15T01:14:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = moarbot-twitch
TEMPLATE = app

DESTDIR = bin


SOURCES += main.cpp\
        mainwindow.cpp \
    files.cpp

HEADERS  += mainwindow.h \
    files.h

FORMS    += mainwindow.ui
