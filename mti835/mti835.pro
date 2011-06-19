#-------------------------------------------------
#
# Project created by QtCreator 2011-06-16T22:20:34
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = mti835
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    GLTexture.cpp \
    qtglut.cpp \
    vmath.c \
    markers_parser.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    GLTexture.h \
    countries.h \
    qtglut.h \
    markers_parser.h \
    vmath.h

FORMS    += mainwindow.ui
