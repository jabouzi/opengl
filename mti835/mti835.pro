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
    log.cpp \
    vmath.c \
    sunpos.c

HEADERS  += mainwindow.h \
    glwidget.h \
    GLTexture.h \
    countries.h \
    qtglut.h \
    log.h \
    vmath.h \
    sunpos.h

FORMS    += mainwindow.ui
