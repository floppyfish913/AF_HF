QT += core network
QT -= gui

TARGET = TCP_try
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tcp_com_thread.cpp \
    message_handler.cpp

HEADERS += \
    tcp_com_thread.h \
    message_handler.h

