#include "visualizer.h"

//#include <GLUT/glut.h>
#include <QtGlobal>
#include <QtDebug>
#include "constants.h"

Visualizer::Visualizer(QWidget *parent) :
     QGLWidget(parent)
{
     setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
     this->_trackball = TrackBall(0.05f, QVector3D(0, 1, 0), TrackBall::Sphere);
     //this->_trackball2 = TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::Plane);
}


void Visualizer::initializeGL()
{
     qglColor(Qt::black);
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glShadeModel(GL_SMOOTH);

     GLfloat lightPos[] = { 0.0, 0.0, -10.0, 0.0};
}

void Visualizer::paintGL()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();
     this->draw();
}

void Visualizer::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

static void multMatrix(const QMatrix4x4& m)
{
     // static to prevent glMultMatrixf to fail on certain drivers
     static GLfloat mat[16];
     const qreal *data = m.constData();
     for (int index = 0; index < 16; ++index)
          mat[index] = data[index];
     glMultMatrixf(mat);
}

static void loadMatrix(const QMatrix4x4& m)
{
     // static to prevent glLoadMatrixf to fail on certain drivers
     static GLfloat mat[16];
     const qreal *data = m.constData();
     for (int index = 0; index < 16; ++index)
          mat[index] = data[index];
     glLoadMatrixf(mat);
}

void getProjectionMatrix(QMatrix4x4& mat, float nearZ, float farZ)
{
     static const QMatrix4x4 reference(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, -1.0f, 0.0f);

     mat = reference;
     mat(2, 2) = (nearZ+farZ)/(nearZ-farZ);
     mat(2, 3) = 2.0f*nearZ*farZ/(nearZ-farZ);
}

void Visualizer::draw()
{
     int i;
     GLint v[24][3]= {
          { 1,  1,  1},
          {-1,  1,  1},
          {-1, -1,  1}, // Front
          { 1, -1,  1},

          { 1,  1,  1},
          { 1,  1, -1},
          {-1,  1, -1}, // Top
          {-1,  1,  1},

          { 1,  1,  1},
          { 1, -1,  1},
          { 1, -1, -1}, // Right
          { 1,  1, -1},

          {-1,  1,  1},
          {-1,  1, -1},
          {-1, -1, -1}, // Left
          {-1, -1,  1},

          {-1, -1,  1},
          {-1, -1, -1}, // Bottom
          { 1, -1, -1},
          { 1, -1,  1},

          { 1,  1, -1},
          { 1, -1, -1},
          {-1, -1, -1}, // Back
          {-1,  1, -1}
     };


     glMatrixMode(GL_PROJECTION);

     glLoadIdentity();
     gluPerspective(60.0, width() / height(), 0.01, 15.0);
     gluLookAt(0.0,0.0,5.0,0,0,0,0,1,0);

     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     {
         QMatrix4x4 m;
         m.rotate(this->_trackball.rotation());
         multMatrix(m);
     }

     /*QQuaternion q = this->_trackball.rotation();
     qDebug() << q;
     glRotatef(180.0*q.scalar() / PI, q.x(), q.y(), q.z());*/

     glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

     for(i=0; i<24; i+=4) {
          glBegin(GL_LINE_LOOP);
          glLineWidth(2.0);
          qglColor(Qt::white);
          glVertex3iv(v[i]);
          glEnd();
     }

     glBegin(GL_QUADS);
     qglColor(Qt::green);
     for(i=0; i<24; i++)
     {
          glVertex3iv(v[i]);
     }
     glEnd();
}

QPointF Visualizer::pixelPosToViewPos(const QPointF& p)
{
     return QPointF(2.0 * float(p.x()) / width() - 1.0,
                         1.0 - 2.0 * float(p.y()) / height());
}

void Visualizer::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        this->_trackball.move(pixelPosToViewPos(e->posF()), QQuaternion());
        updateGL();
        e->accept();
    } else {
        this->_trackball.release(pixelPosToViewPos(e->posF()), QQuaternion());
    }
}

void Visualizer::mousePressEvent(QMouseEvent *e)
{
     if(e->buttons() & Qt::LeftButton)
     {
        this->_trackball.push(pixelPosToViewPos(e->posF()), QQuaternion());
        //this->_trackball.push(pixelPosToViewPos(e->posF()), this->_trackball2.rotation().conjugate());
        e->accept();
     }
}

void Visualizer::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        this->_trackball.release(pixelPosToViewPos(e->posF()),QQuaternion());
        e->accept();
    }
}
