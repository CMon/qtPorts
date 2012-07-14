#-------------------------------------------------
#
# Project created by QtCreator 2012-03-22T16:40:48
#
#-------------------------------------------------

QT       += core gui

TARGET = qtPorts
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    macport.cpp \
    shell.cpp \
    util.cpp \
    packagewidget.cpp

HEADERS  += mainwindow.h \
    macport.h \
    shell.h \
    util.h \
    packagewidget.h

FORMS    += mainwindow.ui \
    packagewidget.ui

RESOURCES += \
    images.qrc
