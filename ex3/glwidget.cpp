#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "qtglut.h"

GLWidget::GLWidget()
{
    startTimer( 5 );
    movingUp = false;
    yLocation = 0.0f;
    yRotationAngle = 0.0f;
}

void GLWidget::initializeGL()
{

}


void GLWidget::paintGL()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glTranslatef(0.0f, yLocation, 0.0f);
    glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f);
    glutWireCube(2.0f);
    glFlush();
    if (movingUp) yLocation -= 0.005f;
    else yLocation += 0.005f;
    if (yLocation < -3.0f) movingUp = false;
    else if (yLocation > 3.0f) movingUp = true;
    yRotationAngle += 0.005f;
    if (yRotationAngle > 360.0f)  yRotationAngle -= 360.0f;
}


void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
    glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
    glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
    glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
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
