#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include "glwidget.h"
#include "qtglut.h"
#include "countries.h"

GLWidget::GLWidget()
{
    idle = true;    
    angleX = 0, angleY = 0;
    rotX = 0, rotY = 0;
    autoRotX = 0, autoRotY = 0;
    scaleAll = 1;
    lineWidth = 1;
    time_ = 0.0;
    skinsList << "images/earth.tga" << "images/earth_elevation.tga" << "images/earth_vector.tga";
    currentSkinId = 1;
    firstMove = false;
    showCountryNames = false;
    showLatLong = false;
    showCurve = false;
    activateLight = false;
    activateTexture = false;
    activateBackground = false;
    showEarth = false;
    animate = false;
}

void GLWidget::initializeGL()
{
    for(int i = 0; i < skinsList.count(); i++ )
    {
        earthTexture.LoadTGA(skinsList.at(i).toLatin1().data());
        idsList[i] = earthTexture.getTextureId();
    }
        // generate our sphere
    for (int x=0; x<=EARTH_LON_RES; x++) {
        for (int y=0; y<=EARTH_LAT_RES; y++) {
            // angle around y-axis (which is x-value)
            float    angX, angY;
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
     //qDebug() << idsList[0] << idsList[1]  << idsList[2] ;
     currentSkinId = idsList[listIndex];
     if (currentSkinId == 0) currentSkinId = 1;
}

void GLWidget::updateEarth()
{
    updateGL();
}

void GLWidget::drawEarth()
{
    if (showEarth)
    {
        if (activateTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, currentSkinId);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        }

        if (!activateLight)
        {
            glColor3f(1,1,1);
        }
        int x, y;
        for (y=0; y<EARTH_LAT_RES; y++) {
            if (!activateTexture)
            {
                glBegin(GL_POINTS);
            }
            else
            {
                glBegin(GL_QUAD_STRIP);
            }
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

        if (showLatLong)
        {
            glEnable(GL_BLEND);
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
            glDisable(GL_BLEND);
        }

        if (activateTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }
    }
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

void GLWidget::lonLat2Point2(float lon, float lat, Vector *pos, int increment = 1000)
{
    // lat -90..90 => Y
    // lon -180..180 => X
    float    angX, angY;
    angX = (180.f+lat) * PI / 180.f;
    angY = lon * PI / 180.f;
    pos->x = fabsf(cosf(angY)) * (EARTH_RADIUS2 + increment) * sinf(angX);
    pos->y = EARTH_RADIUS * sinf(angY);
    pos->z = fabsf(cosf(angY)) * (EARTH_RADIUS2 + increment) * cosf(angX);
}

GLuint GLWidget::loadMipmappedTexture(Image *image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
                      GL_RGB,
                      image->width, image->height,
                      GL_RGB,
                      GL_UNSIGNED_BYTE,
                      image->pixels);
    return textureId;
}

void GLWidget::drawAxis()
{
    if (showAxis)
    {
        //glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glColor4f(0,0,0,1.0f);
        glBegin(GL_LINE);
        glLineWidth(40);
        glVertex3f(10000,0,0);
        glVertex3f(-10000,0,0);
        glVertex3f(0,10000,0);
        glVertex3f(0,-10000,0);
        glVertex3f(0,0,10000);
        glVertex3f(0,0,-10000);
        glEnd();
        QFont myFont( "TypeWriter", 6*scaleAll, QFont::Bold);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        renderText(10000  ,  0  ,  0, "X +", myFont );
        renderText(-10000  ,  0  ,  0, "X -", myFont );
        renderText(  0,  10000  ,  0, "Y +", myFont );
        renderText(0  ,  -10000  ,  0, "Y -", myFont );
        renderText(0  ,  0 ,  10000, "Z +", myFont );
        renderText(0  ,  0 ,  -10000, "Z -", myFont );
        glPopMatrix();
    }
}

void GLWidget::drawNames()
{
    if (showCountryNames)
    {
        glPushMatrix();
        //glEnable(GL_BLEND);
        glLineWidth(1);
        Vector countries_positions[NUM_COUNTRIES];
        Vector countries_positions2[NUM_COUNTRIES];
        QFont myFont( "TypeWriter", 6*scaleAll, QFont::Bold);
        for (int i=0; i<NUM_COUNTRIES-1; i++) {
           lonLat2Point(countries[i].lon, countries[i].lat, &countries_positions[i]);
           lonLat2Point2(countries[i].lon, countries[i].lat, &countries_positions2[i],0);
           glBegin(GL_LINES);
               glColor4f(1,0,0,1.0f);
               glVertex3f (countries_positions[i].x  ,  countries_positions[i].y  ,  countries_positions[i].z);
               glVertex3f (countries_positions2[i].x  ,  countries_positions2[i].y  ,  countries_positions2[i].z);
           glEnd();
           glColor4f(1.0, 1.0, 1.0, 1.0);
           renderText(countries_positions2[i].x  ,  countries_positions2[i].y  ,  countries_positions2[i].z, QString(countries[i].name), myFont );
        }
        //glDisable(GL_BLEND);
        glPopMatrix();
    }
}

void GLWidget::drawNormals()
{

}

void GLWidget::drawCurve()
{
    if (showCurve)
    {
        Vector v1[2];
        Vector v2[2];
        lonLat2Point(21.4273779,39.8148383, &v1[0]);
        lonLat2Point2(21.427377,9.8148383, &v2[0],10000);
        lonLat2Point(45.0, -73.0, &v1[1]);
        lonLat2Point2(45.0, -73.0, &v2[1],10000);

        //glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glBegin(GL_POINTS);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(v1[0].x, v1[0].y, v1[0].z);
        glVertex3f(v1[1].x, v1[1].y, v1[1].z);
        glVertex3f(v2[0].x, v2[0].y, v2[0].z);
        glVertex3f(v2[1].x, v2[1].y, v2[1].z);
        glEnd();

        GLfloat ctrlPts [4][3] = { {v1[0].x, v1[0].y, v1[0].z}, {v2[0].x, v2[0].y, v2[0].z},
                                       {v2[1].x, v2[1].y, v2[1].z},  {v1[1].x, v1[1].y, v1[1].z} };

            glMap1f (GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts);
            glEnable (GL_MAP1_VERTEX_3);

            GLint k;
            glColor3f (1.0, 0.0, 1.0);
            glBegin (GL_LINE_STRIP);             //  Generate Bezier "curve".
                glLineWidth(20);
                for (k = 0; k <= 50; k++)
                    glEvalCoord1f (GLfloat (k) / 50.0);
            glEnd ( );

            glColor3f (1.0, 0.0, 1.0);
            glBegin (GL_POINTS);                 //  Plot control points.
            glPointSize (5.0);                   //  Set point size to 5.0.
                for (k = 0; k < 4; k++);
                        glVertex3fv (&ctrlPts [k][0]);
            glEnd ( );
            glPopMatrix();
    }
}

void GLWidget::drawSatellites()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(45  ,  -73  , 8000);
    glColor3f(1,0,0);
    glutSolidCube(130);
    glTranslatef(33  ,  9  , 8000);
    glColor3f(1,0,0);
    glutSolidCube(130);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
}

void GLWidget::drawStars() {

    if (activateBackground)
    {
        QString path = QCoreApplication::applicationDirPath ();
        if (path.data()[path.size() - 1] != '/') path += "/";
        image = loadBMP("images/stars.bmp");
        _textureId = loadMipmappedTexture(image);
        delete image;

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 1, 0, 1, 0, 1);
        glDepthMask( false );

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glBegin(GL_QUADS);
          glTexCoord2f(-1,-1);
          glVertex2f(-1,-1);

          glTexCoord2f(1,-1);
          glVertex2f(1,-1);

          glTexCoord2f(1,1);
          glVertex2f(1,1);

          glTexCoord2f(-1,1);
          glVertex2f(-1,1);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glDepthMask( true );
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    }
}

void GLWidget::drawLight()
{
    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glLoadIdentity();
    GLfloat light1PosType [] = {0  ,  0  , 0  , 1.0};
    GLfloat light2PosType [] = {0  ,  0  , 8000  , 0.0};
    GLfloat whiteColor [] = {1.0,1.0,1.0,1.0};
    //glPushMatrix();
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteColor);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,whiteColor);
    //glPushMatrix();
    //glTranslatef(1000,0,0);
    //glRotatef(100, 1,0,0);
    glLightfv(GL_LIGHT0, GL_POSITION, light1PosType);
    glLightfv(GL_LIGHT1, GL_POSITION, light2PosType);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glPopMatrix();
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawStars();
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    time_ += 0.1; 
    glTranslatef(0,0,-125);
    glScalef(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE);
    glScalef(scaleAll, scaleAll, scaleAll);
    glRotatef(rotY, 1,0,0);
    glRotatef(rotX, 0,1,0);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
    glEnable(GL_DEPTH_TEST);
    drawEarth();

    if (showCurve)
    {
        drawCurve();
    }

    if (showCountryNames)
    {
        drawNames();
    }

    if (showAxis)
    {
        drawAxis();
    }

    if (activateLight)
    {
        drawLight();
    }
    else
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    glPopMatrix();
    //drawSatellites();
    swapBuffers();
    if (animate)
    {
        rotX++;
    }
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

void GLWidget::startAnimation()
{
    startTimer(30);
}

void GLWidget::StopAnimation()
{
    killTimer(timerId);
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    updateGL();
    timerId = event->timerId();
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
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltX = event->x() - lastPos.x();
    int deltY = event->y() - lastPos.y();     

    if (event->buttons() & Qt::LeftButton) {
        rotX += deltX*0.25f/scaleAll;
        rotY += deltY*0.25f/scaleAll;

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

    lastPos = event->pos();
                // save values for auto rotation
    //autoRotX = deltX*0.25f;
    //autoRotY = deltY*0.25f;

    updateGL();
    emit rotationsChanged();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    angleX = 180 - rotX;
    angleY = rotY;
    idle = true;
    startTimer(10000);
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
