#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "qtglut.h"
#include "countries.h"

GLWidget::GLWidget()
{
    //glClearColor(1.0, 1.0, 1.0, 0.0);
    //glMatrixMode(GL_PROJECTION);
    //gluOrtho2D(0.0, 600, 0.0, 300);
    //startTimer( 30 );
    angleX = 0, angleY = 0;
    rotX = 180, rotY = 0;
    autoRotX = 0, autoRotY = 0;
    scaleAll = 1;
    lineWidth = 1;
    time_ = 0.0;
    skinsList << "images/earth.tga" << "images/earth_elevation.tga" << "images/earth_vector.tga";
    currentSkinId = 1;
    firstMove = false;
    showCountryNames = false;
    showLatLong = false;
}

void GLWidget::initializeGL()
{   
    //skin = skinsList.at(0);
    for(int i = 0; i < skinsList.count(); i++ )
    {
        earthTexture.LoadTGA(skinsList.at(i).toLatin1().data());
        idsList[i] = earthTexture.getTextureId();
        //qDebug() << idsList.at(i) ;
    }
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

void GLWidget::setSkin(int listIndex)
{
    //skin = skinsList.at(listIndex);
     qDebug() << idsList[0] << idsList[1]  << idsList[2] ;
     currentSkinId = idsList[listIndex];
     if (currentSkinId == 0) currentSkinId = 1;
    //earthTexture.LoadTGA(skinsList.at(listIndex).toLatin1().data());
}

void GLWidget::updateEarth()
{
    //initializeGL();
    updateGL();
}

void GLWidget::drawEarth()
{
    float z_prim = 0.0;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, currentSkinId);
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

    /*Vector mycountries[3];
    lonLat2Point(33.886917, 9.537499,  &mycountries[0]);
    qDebug() << mycountries[0].x << " - " << mycountries[0].y << " - " << mycountries[0].z;
    lonLat2Point(42.8333, 12.8333,  &mycountries[1]);
    qDebug() << mycountries[1].x << " - " << mycountries[1].y << " - " << mycountries[1].z;
    lonLat2Point(35.86166, 104.195397,   &mycountries[2]);
    qDebug() << mycountries[2].x << " - " << mycountries[2].y << " - " << mycountries[2].z;

    glEnable(GL_BLEND);
    glLineWidth(1);
    glBegin(GL_LINES);
        glColor4f(1,0,0,1.0f);
        glVertex3f (mycountries[0].x  ,  mycountries[0].y  ,  mycountries[0].z);
        glVertex3f (mycountries[0].x  ,  mycountries[0].y  ,  mycountries[0].z-500);
        glVertex3f (mycountries[1].x  ,  mycountries[1].y  ,  mycountries[1].z);
        glVertex3f (mycountries[1].x  ,  mycountries[1].y  ,  mycountries[1].z-500);
        glVertex3f (mycountries[2].x  ,  mycountries[2].y  ,  mycountries[2].z);
        glVertex3f (mycountries[2].x  ,  mycountries[2].y  ,  mycountries[2].z+500);
        //glutSolidCube(30);
        //glPointSize (50.0);
    glEnd();

    //glDisable(GL_DEPTH_TEST);
    QFont myFont( "TypeWriter", 8, QFont::Bold);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    renderText(mycountries[0].x  ,  mycountries[0].y  ,  mycountries[0].z-500, QString("Tunisia"), myFont );
    renderText(mycountries[1].x  ,  mycountries[1].y  ,  mycountries[1].z-500, QString("Italy"), myFont  );
    renderText(mycountries[2].x  ,  mycountries[2].y  ,  mycountries[2].z+500, QString("China"), myFont  );



   glDisable(GL_BLEND);
*/
    if (showCountryNames)
    {
        glEnable(GL_BLEND);
        glLineWidth(1);
        Vector countries_positions[NUM_COUNTRIES];
        QFont myFont( "TypeWriter", 6*scaleAll, QFont::Bold);
        for (int i=0; i<NUM_COUNTRIES-1; i++) {
           lonLat2Point(countries[i].lon, countries[i].lat, &countries_positions[i]);
           z_prim = -500;
           if (countries_positions[i].z > 0) z_prim = 500;
           glBegin(GL_LINES);
               glColor4f(1,0,0,1.0f);
               glVertex3f (countries_positions[i].x  ,  countries_positions[i].y  ,  countries_positions[i].z);
               glVertex3f (countries_positions[i].x  ,  countries_positions[i].y  ,  countries_positions[i].z+z_prim);
           glEnd();
           glColor4f(1.0, 1.0, 1.0, 1.0);
           renderText(countries_positions[i].x  ,  countries_positions[i].y  ,  countries_positions[i].z+z_prim, QString(countries[i].name), myFont );
        }
        glDisable(GL_BLEND);
    }

    if (showLatLong)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(lineWidth);
        for (y=0; y<=EARTH_LAT_RES; y++) {
           glBegin(GL_LINE_STRIP);
           for (x=0; x<=EARTH_LON_RES; x++) {
               glColor4f(0,0,0,0.5f);
               glVertex3fv((float*)&vertices[x][y]);
           }
          glEnd();
        }

        for (x=0; x<=EARTH_LON_RES; x++) {
           glBegin(GL_LINE_STRIP);
           for (y=0; y<=EARTH_LAT_RES; y++) {
               glColor4f(0,0,0,0.5f);
               glVertex3fv((float*)&vertices[x][y]);
           }
           glEnd();
        }
        glDisable(GL_BLEND);
    }
    glDisable(GL_TEXTURE_2D);

}

void GLWidget::lonLat2Point(float lon, float lat, Vector *pos)
{
    // lat -90..90 => Y
    // lon -180..180 => X
    float    angX, angY;
    angX = (180.f+lat) * PI / 180.f;
    angY = lon * PI / 180.f;
    pos->x = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
    pos->y = EARTH_RADIUS * sinf(angY);
    pos->z = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
}


void GLWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();   
    time_ += 0.1;
    glTranslatef(0,0,-125);
    glScalef(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE);
    glScalef(scaleAll, scaleAll, scaleAll);
    glRotatef(rotY, 1,0,0);
    glRotatef(rotX, 0,1,0);    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
    drawEarth();
    //draw_markers( "xglobe-markers", 0, 0x000000FF, 0x00FFFFFF );
    glPopMatrix();
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
    rotX -= (xAngle - angleX)/scaleAll;
    rotY += (yAngle - angleY)/scaleAll;
    angleX = xAngle;
    angleY = yAngle;
    //zRot += zAngle;
    //qDebug() << angleX << angleY;
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    //angleX = 0;
    //angleY = 0;
    //qDebug() << lastPos;
    //qDebug() << pixelPosToViewPos(lastPos) << endl;   
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    /*if (!firstMove)
    {
        angleX = rotX;
        angleY = rotY;
        qDebug() << "PressEvent : " << angleX << angleY;
        firstMove = true;
    }*/

    int deltX = event->x() - lastPos.x();
    int deltY = event->y() - lastPos.y();     

	if (event->buttons() & Qt::LeftButton) {       
        rotX += deltX*0.25f/scaleAll;
        rotY += deltY*0.25f/scaleAll;
        //qDebug() << angleX << angleY;
		//rotateBy(1 * deltX, 0 , 0);
		//qDebug() << 'x : ' << event->x() << endl;
		//qDebug() << 'y : ' << event->y() << endl;
		//rotateBy(deltX, deltY, 0);
		
	}
	else if (event->buttons() & Qt::RightButton) {
		float addition;
		addition = ((deltX+deltY) / 200.f);
		
		if (addition < 0 && scaleAll+addition > MIN_SCALE) {
			scaleAll += addition;
		}

		if (addition > 0 && scaleAll+addition < MAX_SCALE) {
			scaleAll += addition;
		}
	}
        /*else if (event->buttons() & Qt::RightButton) {
		rotateBy(9.537499, 33.886917, 0);
		qDebug() << "right";
		//rotY = 45.5089; 9.537499
		//rotX = -73.5542; 33.886917
		updateGL();
        }*/
	//
	lastPos = event->pos();
				// save values for auto rotation
	//autoRotX = deltX*0.25f;
	//autoRotY = deltY*0.25f;

	updateGL();
	emit rotationsChanged();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    //emit rotationsChanged();
    angleX = 180 - rotX;
    angleY = rotY;
    //qDebug() << "ReleaseEvent : " << 180 - rotX << rotY << angleX << angleY;
}

float GLWidget::getRotX()
{
	return rotX;
}

float GLWidget::getRotY()
{
	return rotY;
}

float GLWidget::getRotZ()
{
	return 0.0;
}
