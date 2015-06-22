#-------------------------------------------------
#
# Project created by QtCreator 2015-02-27T10:31:09
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Protocol
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addDialog.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    addDialog.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    addDialog.ui \
    aboutdialog.ui

OTHER_FILES += \
    lapa.jpg
