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

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 450


class GLWidget : public QGLWidget
{
Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
};

#endif // __GLWIDGET_H__
