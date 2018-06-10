TEMPLATE = app
CONFIG += console c++11
CONFIG += thread
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

INCLUDEPATH += $$PWD/../cpp-peglib
DEPENDPATH += $$PWD/../cpp-peglib

LIBS += -pthread
