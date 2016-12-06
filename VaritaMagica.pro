#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T20:01:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VaritaMagica
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tgs.cpp

HEADERS  += mainwindow.h \
    utilidades.h \
    tgs.h \
    Grafo.h \
    Nodo.h \
    Arista.h \
    segmentacion.h \
    paper.h

FORMS    += mainwindow.ui
