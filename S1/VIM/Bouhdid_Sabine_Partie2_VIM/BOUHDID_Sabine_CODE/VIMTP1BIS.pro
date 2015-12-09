#-------------------------------------------------
#
# Project created by QtCreator 2015-10-03T16:46:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VIMTP1BIS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    matrice.cpp \
    Util.cpp \
    filtre.cpp \
    histogramme.cpp \
    pointh.cpp \
    points.cpp \
    noeud.cpp \
    geometrie.cpp \
    histodialog.cpp

HEADERS  += mainwindow.h \
    matrice.h \
    Util.h \
    filtre.h \
    histogramme.h \
    pointh.h \
    points.h \
    noeud.h \
    geometrie.h \
    histodialog.h

FORMS    += mainwindow.ui \
    histodialog.ui
