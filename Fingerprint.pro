#-------------------------------------------------
#
# Project created by QtCreator 2016-07-12T01:22:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fingerprint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fprintpp.cpp \
    countedbody.cpp

HEADERS  += mainwindow.h \
    fprintpp.h \
    countedbody.h

FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += libfprint
