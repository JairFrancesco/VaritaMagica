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
    tgs.cpp \
    utility.cpp

HEADERS  += mainwindow.h \
    utilidades.h \
    tgs.h \
    Grafo.h \
    Nodo.h \
    Arista.h \
    segment-image.h \
    disjoint-set.h \
    segment-graph.h \
    convolve.h \
    utility.h \
    pnmfile.h \
    misc.h \
    imutil.h \
    imconv.h \
    image.h \
    filter.h

FORMS    += mainwindow.ui
