TEMPLATE = app
CONFIG += console c++17
CONFIG += thread
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

INCLUDEPATH += $$PWD/../cpp-peglib
DEPENDPATH += $$PWD/../cpp-peglib

LIBS += -pthread
QMAKE_CXXFLAGS += -std=c++14
