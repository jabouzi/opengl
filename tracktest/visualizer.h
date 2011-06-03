#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QGLWidget>
#include "trackball.h"

class Visualizer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Visualizer(QWidget *parent = 0);

signals:

public slots:

protected:
    void initializeGL();

    void paintGL();

    void resizeGL(int w, int h);

    void setProjection();

    void draw();

    void mouseMoveEvent(QMouseEvent *e);

    void mousePressEvent(QMouseEvent *);

    void mouseReleaseEvent(QMouseEvent *);

    QPointF pixelPosToViewPos(const QPointF&);

    TrackBall _trackball;
};

#endif // VISUALIZER_H
