#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"


void GLWidget::initializeGL()
{
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glMatrixMode(GL_PROJECTION);
        gluOrtho2D(0.0, 200, 0.0, 150);
}


void GLWidget::paintGL()
{
     glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
            glVertex2i(180, 15);
            glVertex2i(10, 145);
        glEnd();

        QFont myFont( "TypeWriter", 10, QFont::Bold);

        glColor4f(1.0, 0.0, 0.0, 1.0);
        renderText(20, 20, QString("Hello World"), myFont );

        glFlush();
}


void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}
