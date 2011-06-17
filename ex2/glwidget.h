#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <QGLWidget>

#define WINDOW_WIDTH  250
#define WINDOW_HEIGHT 250
#define PI 3.141592654
#define TWOPI 6.283185308


struct XYZ
{
	int x,y,z;
};


class GLWidget : public QGLWidget
{
Q_OBJECT

public:
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void CreateSphere(XYZ, double, int);
};

#endif // __GLWIDGET_H__
