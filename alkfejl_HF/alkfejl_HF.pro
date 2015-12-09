TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    applicationwindow.cpp \
    message_handler.cpp \
    quadro_msg.cpp \
    tcp_com.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    applicationwindow.h \
    message_handler.h \
    quadro_msg.h \
    tcp_com.h

