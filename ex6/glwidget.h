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
    GLfloat angle;

	void initializeGL();
    void cube();
	void paintGL();
    void resizeGL(int width, int height);
    void timerEvent(QTimerEvent *event);

};

#endif // __GLWIDGET_H__
