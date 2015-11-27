#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T05:55:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SkillEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    condition.cpp \
    class.cpp \
    skill.cpp \
    function.cpp \
    actor.cpp \
    enemy.cpp \
    dynamic.cpp \
    gameobject.cpp \
    global.cpp

HEADERS  += mainwindow.h \
    condition.h \
    class.h \
    skill.h \
    function.h \
    actor.h \
    enemy.h \
    dynamic.h \
    gameobject.h \
    global.h

FORMS    += mainwindow.ui
