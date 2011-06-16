#-------------------------------------------------
#
# Project created by QtCreator 2011-05-17T22:56:52
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = ex8
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    glwidget.cpp \
    qtglut.cpp \
    GLTexture.cpp \
    trackball.cpp

HEADERS  += widget.h \
    glwidget.h \
    qtglut.h \
    GLTexture.h \
    trackball.h \
    countries.h

FORMS    += widget.ui
