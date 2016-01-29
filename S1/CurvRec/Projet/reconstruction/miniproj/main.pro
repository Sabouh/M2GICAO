TARGET = pointsToSurface
CONFIG *= qt opengl release
QT *= opengl xml

LIBS += -Wl,-rpath=../../lib/libQGLViewer-2.6.1/QGLViewer -L../../lib/libQGLViewer-2.6.1/QGLViewer -lQGLViewer
INCLUDEPATH += ../../lib/libQGLViewer-2.6.1/
HEADERS = geom.h iso_value.h data_struct_algo.h pointsToSurface.h eventWidget.h viewer.h
SOURCES = geom.cpp iso_value.cpp data_struct_algo.cpp pointsToSurface.cpp eventWidget.cpp viewer.cpp main.cpp

#LIBS *= -lqglviewer-qt4

