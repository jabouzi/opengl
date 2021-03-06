#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <QGLWidget>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "GLTexture.h"
#include "trackball.h"
#include <iostream>
using namespace::std;

//#define WINDOW_WIDTH  500
//#define WINDOW_HEIGHT 500
#define PI 3.14159265
#define EARTH_LON_RES    60    // Longitude Resolution (x)
#define EARTH_LAT_RES    60    // Latitude Resolution (y)
#define EARTH_RADIUS    6378    // in kilometers
#define WORLD_SCALE        0.01f    // scale of all the world
#define MIN_SCALE        0.5f    // how much can we zoom out
#define MAX_SCALE        1.9f    // and in

struct Vector {
    float x, y, z;
};

struct Mapping {
    float u, v;
};

class GLWidget : public QGLWidget
{
Q_OBJECT

public:
    GLWidget();
    QSize minimumSizeHint() const;
    //QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    float time_;

protected:    
    QPoint lastPos;
    GLfloat angle;
    CGLTexture earthTexture;
    Vector vertices[EARTH_LON_RES+1][EARTH_LAT_RES+1];
    Mapping mapping[EARTH_LON_RES+1][EARTH_LAT_RES+1];
    float rotX, rotY, autoRotX, autoRotY, scaleAll;
    TrackBall _trackball;
    int lineWidth;
    float temp1;
    float temp2;

    void initializeGL();
    void paintGL();
    void resizeGL(int, int);
    void timerEvent(QTimerEvent *event);
    void DrawEarth();
    void LonLat2Point(float , float , Vector *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    QPointF pixelPosToViewPos(const QPointF&);
};

#endif // __GLWIDGET_H__
