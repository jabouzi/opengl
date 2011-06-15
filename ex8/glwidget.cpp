#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "qtglut.h"

GLWidget::GLWidget()
{
    startTimer( 30 );
    angle = 45.0;
    rotX = 140, rotY = 0;
    autoRotX = 0, autoRotY = 0;
    scaleAll = 1;
    lineWidth = 1;
}

void GLWidget::initializeGL()
{
    earthTexture.LoadTGA("images/earth_vector.tga");
        // generate our sphere
    for (int x=0; x<=EARTH_LON_RES; x++) {
        for (int y=0; y<=EARTH_LAT_RES; y++) {
            // angle around y-axis (which is x-value)
            float	angX, angY;
			angX = (x * 360.f / EARTH_LON_RES) * PI / 180.f;
			angY = (-90.f + (y * 180.f / EARTH_LAT_RES)) * PI / 180.f;
			vertices[x][y].x = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
			vertices[x][y].y = EARTH_RADIUS * sinf(angY);
			vertices[x][y].z = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
			mapping[x][y].u = (float)x / EARTH_LON_RES;
			mapping[x][y].v = (float)y / EARTH_LAT_RES;
        }
    }
}

void GLWidget::DrawEarth()
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    earthTexture.Use();
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glColor3f(1,1,1);
    int x, y;
    for (y=0; y<EARTH_LAT_RES; y++) {
		glBegin(GL_QUAD_STRIP);
        for (x=0; x<EARTH_LON_RES; x++) {

            glTexCoord2fv((GLfloat*)&mapping[x][y]);
            glVertex3fv((GLfloat*)&vertices[x][y]);
            glTexCoord2fv((GLfloat*)&mapping[x][y+1]);
            glVertex3fv((GLfloat*)&vertices[x][y+1]);
            glTexCoord2fv((GLfloat*)&mapping[x+1][y]);
            glVertex3fv((GLfloat*)&vertices[x+1][y]);
            glTexCoord2fv((GLfloat*)&mapping[x+1][y+1]);
            glVertex3fv((GLfloat*)&vertices[x+1][y+1]);
        }
		glEnd();
    }
    
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(lineWidth);

    for (y=0; y<=EARTH_LAT_RES; y++) {
        glBegin(GL_LINE_STRIP);
        for (x=0; x<=EARTH_LON_RES; x++) {
            glColor4f(0,0,0,1.0f);
            glVertex3fv((float*)&vertices[x][y]);
        }
        glEnd();
    }

    for (x=0; x<=EARTH_LON_RES; x++) {
        glBegin(GL_LINE_STRIP);
        for (y=0; y<=EARTH_LAT_RES; y++) {
            glColor4f(0,0,0,1.0f);
            glVertex3fv((float*)&vertices[x][y]);
        }
        glEnd();
    }

    glDisable(GL_BLEND);*/
    glDisable(GL_TEXTURE_2D);

}

void GLWidget::LonLat2Point(float lon, float lat, Vector *pos)
{
    // lon -90..90
    // lat -180..180
    float    angX, angY;
    angX = (180.f+lat) * PI / 180.f;
    angY = lon * PI / 180.f;
    pos->x = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
    pos->y = EARTH_RADIUS * sinf(angY);
    pos->z = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
}


void GLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    glTranslatef(0,0,-125);
    glScalef(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE);
    glScalef(scaleAll, scaleAll, scaleAll);
    glRotatef(rotY, 1,0,0);
    glRotatef(rotX, 0,1,0);
    GLdouble modelview_matrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
    DrawEarth();
    glPopMatrix();
    //rotY++;
    //rotX++;
}


void GLWidget::resizeGL(int width, int height)
{
    glViewport (0,0,width,height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0,(GLfloat)width/(GLfloat)height, 0.1, 120.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
   return QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    updateGL();
}

QPointF GLWidget::pixelPosToViewPos(const QPointF& p)
{

     return QPointF(2.0 * float(p.x()) / width() - 1.0, 1.0 - 2.0 * float(p.y()) / height());
}


void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    rotX += xAngle;
    rotY += yAngle;
    //zRot += zAngle;
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    qDebug() << lastPos << endl;
    qDebug() << pixelPosToViewPos(lastPos) << endl;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltX = event->x() - lastPos.x();
    int deltY = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		rotX += deltX*0.25f/scaleAll;
		rotY += deltY*0.25f/scaleAll;
		//rotateBy(1 * deltX, 0 , 0);
		//qDebug() << 'x : ' << event->x() << endl;
		//qDebug() << 'y : ' << event->y() << endl;
		//rotateBy(deltX, deltY, 0);
		
	}
	else if (event->buttons() & Qt::MidButton) {
		float addition;
		addition = ((deltX+deltY) / 200.f);
		
		if (addition < 0 && scaleAll+addition > MIN_SCALE) {
			scaleAll += addition;
		}

		if (addition > 0 && scaleAll+addition < MAX_SCALE) {
			scaleAll += addition;
		}
	}
	//
	lastPos = event->pos();
				// save values for auto rotation
	//autoRotX = deltX*0.25f;
	//autoRotY = deltY*0.25f;

	updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
	//emit clicked();
}

/*
void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        this->_trackball.move(pixelPosToViewPos(e->posF()), QQuaternion());
        updateGL();
        e->accept();
    } else {
        this->_trackball.release(pixelPosToViewPos(e->posF()), QQuaternion());
    }
    rotX += deltX*0.25f/scaleAll;
    rotY += deltY*0.25f/scaleAll;

                // save values for auto rotation
    autoRotX = p.x()*0.25f;
    autoRotY = p.x()*0.25f;
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
     if(e->buttons() & Qt::LeftButton)
     {
        this->_trackball.push(pixelPosToViewPos(e->posF()), QQuaternion());
        this->_trackball.push(pixelPosToViewPos(e->posF()), this->_trackball.rotation().conjugate());
        e->accept();
     }
    rotX += autoRotX*scaleAll*0.1f;
    rotY += autoRotY*scaleAll*0.1f;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        this->_trackball.release(pixelPosToViewPos(e->posF()),QQuaternion());
        e->accept();
    }
}*/
