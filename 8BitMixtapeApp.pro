#-------------------------------------------------
#
# Project created by QtCreator 2014-02-20T11:23:36
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += script

macx:  LIBS += -framework CoreFoundation
macx:  LIBS += -framework IOKit
macx:  LIBS += -L/usr/local/lib -lusb-1.0


INCLUDEPATH += /usr/local/include/libusb-1.0
DEPENDPATH  += /usr/local/include/libusb-1.0


CONFIG += x86 x86_64

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 8BitMixtapeApp
TEMPLATE = app

SOURCES += main.cpp\
        dialog.cpp \
    noisegen.cpp \
    usbaspdetector.cpp

HEADERS  += dialog.h \
    noisegen.h \
    usbaspdetector.h

FORMS    += dialog.ui
