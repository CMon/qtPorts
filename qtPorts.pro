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
    packagewidget.cpp \
    dependencywidget.cpp \
    dependencydialog.cpp \
    cleanupdialog.cpp

HEADERS  += mainwindow.h \
    macport.h \
    shell.h \
    util.h \
    packagewidget.h \
    dependencywidget.h \
    dependencydialog.h \
    cleanupdialog.h

FORMS    += mainwindow.ui \
    packagewidget.ui \
    dependencywidget.ui \
    dependencydialog.ui \
    cleanupdialog.ui

RESOURCES += \
    images.qrc
