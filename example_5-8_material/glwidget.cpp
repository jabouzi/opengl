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


// init
void GLWidget::initializeGL()
{
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.0, 3.0, 2.0, 0.0 };
	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat local_view[] = { 0.0 };

	glClearColor(0.0, 0.1, 0.1, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

/* Draw twelve spheres in 3 rows with 4 columns.
 * The spheres in the first row have materials with no ambient reflection.
 * The second row has materials with significant ambient reflection.
 * The third row has materials with colored ambient reflection.
 *
 * The first column has materials with blue, diffuse reflection only.
 * The second column has blue diffuse reflection, as well as specular
 * reflection with a low shininess exponent.
 * The third column has blue diffuse reflection, as well as specular
 * reflection with a high shininess exponent (a more concentrated highlight).
 * The fourth column has materials which also include an emissive component.
 *
 * glTranslatef() is used to move spheres to their appropriate locations.
 */
void GLWidget::paintGL()
{
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat no_shininess[] = { 0.0 };
	GLfloat low_shininess[] = { 5.0 };
	GLfloat high_shininess[] = { 100.0 };
	GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw sphere in first row, first column
	 * diffuse reflection only; no ambient or specular
	 */
	glPushMatrix();
	glTranslatef (-3.75, 3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in first row, second column
	 * diffuse and specular reflection; low shininess; no ambient
	 */
	glPushMatrix();
	glTranslatef (-1.25, 3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in first row, third column
	 * diffuse and specular reflection; high shininess; no ambient
	 */
	glPushMatrix();
	glTranslatef (1.25, 3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in first row, fourth column
	 * diffuse reflection; emission; no ambient or specular reflection
	 */
	glPushMatrix();
	glTranslatef (3.75, 3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in second row, first column
	 * ambient and diffuse reflection; no specular
	 */
	glPushMatrix();
	glTranslatef (-3.75, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in second row, second column
	 * ambient, diffuse and specular reflection; low shininess
	 */
	glPushMatrix();
	glTranslatef (-1.25, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in second row, third column
	 * ambient, diffuse and specular reflection; high shininess
	 */
	glPushMatrix();
	glTranslatef (1.25, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in second row, fourth column
	 * ambient and diffuse reflection; emission; no specular
	 */
	glPushMatrix();
	glTranslatef (3.75, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in third row, first column
	 * colored ambient and diffuse reflection; no specular
	 */
	glPushMatrix();
	glTranslatef (-3.75, -3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in third row, second column
	 * colored ambient, diffuse and specular reflection; low shininess
	 */
	glPushMatrix();
	glTranslatef (-1.25, -3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in third row, third column
	 * colored ambient, diffuse and specular reflection; high shininess
	 */
	glPushMatrix();
	glTranslatef (1.25, -3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	/* draw sphere in third row, fourth column
	 * colored ambient and diffuse reflection; emission; no specular
	 */
	glPushMatrix();
	glTranslatef (3.75, -3.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glutSolidSphere(1.0, 32, 16);
	glPopMatrix();

	glFlush();
}

// reshape()
void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= (h * 2))
		glOrtho (-6.0, 6.0, -3.0*((GLfloat)h*2)/(GLfloat)w,
			3.0*((GLfloat)h*2)/(GLfloat)w,
			-10.0, 10.0);
	else
		glOrtho (-6.0*(GLfloat)w/((GLfloat)h*2),
			6.0*(GLfloat)w/((GLfloat)h*2),
			-3.0, 3.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}
