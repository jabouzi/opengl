/*
 * Copyright (c) 1993-2003, Silicon Graphics, Inc.
 * All Rights Reserved
 *
 * please review readme.txt which comes with this software
 * or review http://lab.bachem-it.com/opengl/redbook/readme.txt
 * for a detailed copyright notice and warranty disclaimer.
 *
 * get a copy of the complete source code at
 *   http://www.opengl-redbook.com/code/
 *   http://lab.bachem-it.com/opengl/redbook/
 *
 */

#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "qtglut.h"


void GLWidget::initializeGL()
{

}


void GLWidget::paintGL()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our <a title="window" href="http://www.swiftless.com/tutorials/opengl/window.html">window</a> to red
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -5.0f); // <a title="Push" href="http://www.swiftless.com/tutorials/opengl/pop_and_push_matrices.html">Push</a> eveything 5 units back into the scene, otherwise we won't see the primitive
    glutWireCube(2.0f); // Render the primitive
    glFlush(); // Flush the OpenGL buffers to the window
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
