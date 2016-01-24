
# put your path to eigen here! 
#INCLUDEPATH += /home/v/vergner/eigen3
INCLUDEPATH += /usr/include/eigen3

TARGET   = imageMLS
HEADERS  = mlsRec.h imgFilter.h viewer.h
SOURCES  = mlsRec.cpp viewer.cpp main.cpp

QT += core gui widgets
