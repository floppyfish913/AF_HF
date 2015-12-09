QT += core network
QT -= gui

TARGET = servergtester
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    message_handler.cpp

HEADERS += \
    message_handler.h

