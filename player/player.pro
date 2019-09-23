#-------------------------------------------------
#
# Project created by QtCreator 2019-09-17T11:25:57
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player
TEMPLATE = app


SOURCES += main.cpp\
        musicapp.cpp \
    fs.cpp \
    db.cpp \
    song.cpp

HEADERS  += musicapp.h \
    fs.h \
    db.h \
    song.h \
    statuses.h

FORMS    += musicapp.ui
