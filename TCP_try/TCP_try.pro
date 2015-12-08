QT += core network
QT -= gui

TARGET = TCP_try
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    message_handler.cpp \
    quadro_msg.cpp \
    tcp_com.cpp

HEADERS += \
    message_handler.h \
    quadro_msg.h \
    tcp_com.h

