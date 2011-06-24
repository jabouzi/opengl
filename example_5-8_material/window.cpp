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

#include "glwidget.h"
#include "window.h"

Window::Window()
{
	glWidget = new GLWidget;

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(glWidget);

	setCentralWidget(glWidget);
	setWindowTitle(tr("Example 5-8 - material"));
}

void Window::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Escape:
			QCoreApplication::exit();
			break;

		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
			// glWidget->updateGL();
			break;
	}
}

void Window::mousePressEvent (QMouseEvent * event)
{
	switch (event->button()) {
		case Qt::LeftButton:
			// glWidget->updateGL();
			break;

		case Qt::MidButton:
		case Qt::RightButton:
			break;
		default:
			break;
	}
}

void Window::mouseReleaseEvent (QMouseEvent * event)
{
	switch (event->button()) {
		case Qt::LeftButton:
			break;
		case Qt::MidButton:
		case Qt::RightButton:
			break;
	}
}

