#-------------------------------------------------
#
# Project created by QtCreator 2013-02-23T23:41:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sampler
TEMPLATE = app


SOURCES += main.cpp\
        workarea.cpp \
    directorydialog.cpp \
    imagepixmap.cpp \
    resizer.cpp \
    cropper.cpp \
    samplingoptions.cpp \
    counter.cpp \
    saveoptions.cpp

HEADERS  += workarea.hpp \
    directorydialog.hpp \
    imagepixmap.hpp \
    resizer.hpp \
    cropper.hpp \
    samplingoptions.hpp \
    counter.hpp \
    saveoptions.hpp \
    defines.hpp

FORMS    += workarea.ui \
    directorydialog.ui \
    samplingoptions.ui \
    saveoptions.ui
