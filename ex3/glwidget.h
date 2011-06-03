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

#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <QGLWidget>

#define WINDOW_WIDTH  250
#define WINDOW_HEIGHT 250

class GLWidget : public QGLWidget
{
Q_OBJECT

public:
    GLWidget();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
    bool movingUp;
    float yLocation;
    float yRotationAngle;

	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
    void timerEvent(QTimerEvent *event);
};

#endif // __GLWIDGET_H__
