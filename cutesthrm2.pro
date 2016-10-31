QT += core
QT -= gui

QT = core bluetooth widgets

CONFIG += c++11

TARGET = cutesthrm2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sthrm2.cpp

HEADERS += \
    sthrm2.h
