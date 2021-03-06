#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <QGLWidget>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "imageloader.h"
#include "GLTexture.h"
//#include "log.h"
#include <iostream>
using namespace::std;

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500
#define PI 3.14159265
#define EARTH_LON_RES    60    // Longitude Resolution (x)
#define EARTH_LAT_RES    60    // Latitude Resolution (y)
#define EARTH_RADIUS    6378    // in kilometers
#define EARTH_RADIUS2   6878    // in kilometers
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
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setSkin(int);
    float getRotX();
    float getRotY();
    float getRotZ();
    void updateEarth();
    void startAnimation();
    void StopAnimation();
    bool showCountryNames;
    bool showLatLong;
    bool showCurve;
    bool showAxis;
    bool activateLight;
    bool activateTexture;
    bool activateBackground;
    bool showEarth;
    bool animate;
    //Log *pLog;

private:
    float time_;
    QPoint lastPos;
    float angleX, angleY;
    CGLTexture earthTexture;
    Vector vertices[EARTH_LON_RES+1][EARTH_LAT_RES+1];
    Mapping mapping[EARTH_LON_RES+1][EARTH_LAT_RES+1];
    float rotX, rotY, autoRotX, autoRotY, scaleAll;
    QStringList skinsList;
    int idsList[3];
    int currentSkinId;
    int lineWidth;
    QString skin;
    GLdouble modelview_matrix[16];
    bool firstMove;    
    int timerId;
    bool idle;
    int _textureId;
    Image* image;

    void initializeGL();
    void paintGL();
    void resizeGL(int, int);
    void timerEvent(QTimerEvent *event);
    GLuint loadMipmappedTexture(Image *);
    void drawStars();
    void drawEarth();
    void drawAxis();
    void drawNames();
    void drawNormals();
    void drawCurve();
    void drawSatellites();
    void drawLight();
    void lonLat2Point(float , float , Vector *);
    void lonLat2Point2(float , float , Vector *, int);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    QPointF pixelPosToViewPos(const QPointF&);    

signals:
     void rotationsChanged();
};

#endif // __GLWIDGET_H__
