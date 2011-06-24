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

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <QWidget>
#include <QMainWindow>
#include "glwidget.h"


class Window : public QMainWindow
{
	Q_OBJECT

public:
	Window();

private:
	GLWidget *glWidget;

protected:
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent (QMouseEvent * event);
	void mouseReleaseEvent (QMouseEvent * event);
};

#endif // __WINDOW_H__
