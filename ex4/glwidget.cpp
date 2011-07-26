#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "qtglut.h"

GLWidget::GLWidget()
{
    startTimer( 5 );
    angle = 0.0;
}

void GLWidget::initializeGL()
{

}

void GLWidget::cube (void) {
    //glRotatef(angle, 1.0, 0.0, 0.0); //rotate on the x axis
    glRotatef(angle, 0.0, 1.0, 0.0); //rotate on the y axis
    //glRotatef(angle, 0.0, 0.0, 1.0); //rotate on the z axis
    //glColor3f(1.0, 0.0, 0.0);
    glutWireCube(2);
}


void GLWidget::paintGL()
{
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT);
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
    updateGL();
}
