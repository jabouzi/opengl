#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "qtglut.h"

GLWidget::GLWidget()
{
    startTimer( 15 );
    angle = 45.0;
}

void GLWidget::initializeGL()
{
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
}

void GLWidget::cube (void) {
    glRotatef(angle, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidCube(2);
}


void GLWidget::paintGL()
{
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    cube();
    angle ++;
}


void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    //updateGL();
}
