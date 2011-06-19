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

void GLWidget::draw_markers(char * markerfile, int bitmap_mode, unsigned mcol, unsigned lcol)
{
    static Marker * marker;
    static int init=0;
    static int markernr;
    int i;
    double th,ph;
    static int markers_id;
//    static float matrix[16];
    VMvect bx, by;
    float marker_h = 0.02;
    float m[16];
    float vx,vy,vz;

    if(!init){
        init=1;
        /*if(markerfile!=(char *)NULL){
            marker=load_markers(markerfile,&markernr);
        } else {
            marker=load_markers("xglobe-markers",&markernr);
        }*/
//        marker=load_markers("earth-markers-schaumann",&markernr);
/*#ifdef DEBUG
        fprintf(stderr,"markernr=%d\n",markernr);
        fprintf(stderr,"marker[0].name=%s\n",marker[0].name);
        fprintf(stderr,"marker[N-1].name=%s\n",marker[markernr-1].name);
#endif*/
        markers_id = glGenLists(1);
        glNewList(markers_id, GL_COMPILE);
//        glColor3f((double)((mcol>>16) & 0xFF)/255.0,
//                  (double)((mcol>> 8) & 0xFF)/255.0,
//                  (double)((mcol>> 0) & 0xFF)/255.0);
       // glColor4ubv((GLubyte *)&mcol);
        //glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINES);

        for (int i=0; i<NUM_COUNTRIES-1; i++) {
            th=(90.0-countries[i].lon)*M_PI/180.0;
            ph=countries[i].lat*M_PI/180.0;
            glVertex3f(0.25*sin(th)*cos(ph),
                       0.25*sin(th)*sin(ph),
                       0.25*cos(th));
            glVertex3f((0.25+marker_h)*sin(th)*cos(ph),
                       (0.25+marker_h)*sin(th)*sin(ph),
                       (0.25+marker_h)*cos(th));
        }
        glEnd();
        //glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEndList();
    }

    glCallList(markers_id);

//    glColor3f((double)((lcol>>16) & 0xFF)/255.0,
//              (double)((lcol>> 8) & 0xFF)/255.0,
//              (double)((lcol>> 0) & 0xFF)/255.0);
    //glColor4ubv((GLubyte *)&lcol);
    //glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    /*if(!bitmap_mode){
        bx=vec_xyz(1.0,0.0,0.0);
        by=vec_xyz(0.15,1.0,0.0);
        glGetFloatv( GL_MODELVIEW_MATRIX, m );
        vx=bx.x; vy=bx.y; vz=bx.z;
        bx.x=vx*m[0]+vy*m[1]+vz*m[2];
        bx.y=vx*m[4]+vy*m[5]+vz*m[6];
        bx.z=vx*m[8]+vy*m[9]+vz*m[10];
        vx=by.x; vy=by.y; vz=by.z;
        by.x=vx*m[0]+vy*m[1]+vz*m[2];
        by.y=vx*m[4]+vy*m[5]+vz*m[6];
        by.z=vx*m[8]+vy*m[9]+vz*m[10];
    }*/
    //for(i=0;i<markernr;i++){
        //th=(90.0-marker[i].lon)*M_PI/180.0;
        //ph=marker[i].lat*M_PI/180.0;
        //myGlWrite(
                  //vec_xyz((0.25+marker_h*1.1)*sin(th)*cos(ph),
                  //        (0.25+marker_h*1.1)*sin(th)*sin(ph),
                  //        (0.25+marker_h*1.1)*cos(th)),
                  //bx, by,
                  //0,  /* 1/0: take screen/absolute directions for bx, by */
                  //0.005,
                  //bitmap_mode,  /* bitmap mode */
                  //marker[i].name
                 //);
    //}
    //glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}


void GLWidget::drawEarth()
{
     double th,ph;
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

    Vector	country_names_pos[NUM_COUNTRIES];
    for (int i=0; i<NUM_COUNTRIES-1; i++) {

                lonLat2Point(countries[i].lon, countries[i].lat, &country_names_pos[i]);
               // cout << country_names_pos[i].x << " - " << country_names_pos[i].y << endl;
                glPushMatrix();
               // qDebug() << country_names_pos[i].x <<  country_names_pos[i].y << country_names_pos[i].z;
                glTranslatef(country_names_pos[i].x, country_names_pos[i].y, country_names_pos[i].z);
                glColor3f(0,1,1);
                float radius;
                radius = 20+10*sinf(i+time_);
                //qDebug() << radius;
                glutSolidCube( radius);

               // glColor3f(1,0,1); glBegin(GL_TRIANGLES);//glBegin(GL_LINES);
                 /*glVertex3f(0.25*sin(th)*cos(ph),
                            0.25*sin(th)*sin(ph),
                            0.25*cos(th));
                 glVertex3f((0.25+ 0.02)*sin(th)*cos(ph),
                            (0.25+ 0.02)*sin(th)*sin(ph),
                            (0.25+ 0.02)*cos(th));*/
                // glVertex2f(0,0);
                // glVertex2f(10,10);
               //  glEnd();

                glPopMatrix();
            }
     glColor3f(1,1,1);
     //renderText(0, 0,0,"Skander");
   /* GLdouble projection_matrix[16];
            GLint	 viewport[4];
            glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
            glGetIntegerv(GL_VIEWPORT, viewport);

            // goto ortho
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, 1, 0, 1, 1, 129);
            glMatrixMode(GL_MODELVIEW);

            for (int i=0; i<NUM_COUNTRIES-1; i++) {
                GLdouble winX, winY, winZ;

                gluProject(country_names_pos[i].x, country_names_pos[i].y, country_names_pos[i].z,
                            modelview_matrix,
                            projection_matrix,
                            viewport,
                            &winX, &winY, &winZ);
                //glColor4f(1,1,1,.5f);
                //renderText(winX, winY,0,countries[i].name );

                // now isn't this the nastiest trick
                if (winZ < 0.999f) {
                //    glColor4f(1,1,1,.5f);
                    //~ fontShadow();
                    //~ fontShadowColor(0,0,0);
                    //~ fontSize(10+4000*(0.999-winZ));
                    //~ fontDrawString(winX, winY, countries[i].name);
                    renderText(winX, winY,winZ,countries[i].name );
                }
            }

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);*/
    //glPushMatrix();
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(lineWidth);
    glBegin(GL_LINES);
    glColor3f(1,0,0);

    /*for (int i=0; i<NUM_COUNTRIES-1; i++) {
        lonLat2Point(countries[i].lon, countries[i].lat, &country_names_pos[i]);
       // cout << country_names_pos[i].x << " - " << country_names_pos[i].y << endl;
        //glPushMatrix();

        glTranslatef(country_names_pos[i].x, country_names_pos[i].y, country_names_pos[i].z);
        th=(90.0-countries[i].lon)*M_PI/180.0;
        ph=countries[i].lat*M_PI/180.0;
       // glutSolidCube(0.1);
       glBegin(GL_TRIANGLES);//glBegin(GL_LINES);
        glVertex3f(0.25*sin(th)*cos(ph),
                   0.25*sin(th)*sin(ph),
                   0.25*cos(th));
        glVertex3f((0.25+ 0.02)*sin(th)*cos(ph),
                   (0.25+ 0.02)*sin(th)*sin(ph),
                   (0.25+ 0.02)*cos(th));
        glVertex2f(0,0);
        glVertex2f(10,10);
        glEnd();
        //glPopMatrix();
    }*/
    /*glEnd();
    glDisable(GL_BLEND);
    //glPopMatrix();
    //qDebug() << country_names_pos.x << country_names_pos.y << endl;
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(lineWidth);*/

    //for (y=0; y<=EARTH_LAT_RES; y++) {
       // glBegin(GL_LINE_STRIP);
        //for (x=0; x<=EARTH_LON_RES; x++) {
           // glColor4f(0,0,0,1.0f);
           // glVertex3fv((float*)&vertices[x][y]);
            //qDebug() << vertices[x][y].x << vertices[x][y].y << vertices[x][y].z;
        //}
       // glEnd();
    //}
/*
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
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    qDebug() << lastPos;
    //qDebug() << pixelPosToViewPos(lastPos) << endl;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltX = event->x() - lastPos.x();
    int deltY = event->y() - lastPos.y();
     qDebug() << pixelPosToViewPos(lastPos) << endl;

	if (event->buttons() & Qt::LeftButton) {
		angleX = deltX*0.25f/scaleAll;
		angleY = deltY*0.25f/scaleAll;
		rotX += angleX;
		rotY += angleY;

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
