#-------------------------------------------------
#
# Project created by QtCreator 2016-01-21T21:08:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parseur.cpp \
    csv_meteofranceparser.cpp \
    point.cpp

HEADERS  += mainwindow.h \
    parseur.h \
    csv_meteofranceparser.h \
    point.h

FORMS    += mainwindow.ui
