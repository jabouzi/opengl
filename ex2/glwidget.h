#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <QGLWidget>

#define WINDOW_WIDTH  250
#define WINDOW_HEIGHT 250

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
};

#endif // __GLWIDGET_H__
