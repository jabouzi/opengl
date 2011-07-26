// earth-simulation by Ohad Eder Pressman, 2001
// ohad@visual-i.com / ohad.visual-i.com



#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;
#include "GL/glut.h"
#include "GLTexture.h"


// defines
#define PI 3.14159265
#define EARTH_LON_RES	60	// Longitude Resolution (x)
#define EARTH_LAT_RES	60	// Latitude Resolution (y)
#define EARTH_RADIUS	6378	// in kilometers
#define WORLD_SCALE		0.01f	// scale of all the world
#define MIN_SCALE		0.5f	// how much can we zoom out
#define MAX_SCALE		1.9f	// and in

// structures
struct Vector {
	float x, y, z;
};

struct Mapping {
	float u, v;
};

// variables
CGLTexture earthTexture;  
Vector	vertices[EARTH_LON_RES+1][EARTH_LAT_RES+1];
Mapping mapping[EARTH_LON_RES+1][EARTH_LAT_RES+1];
float rotX = 140, rotY = 0;
float autoRotX = 0, autoRotY = 0;
double angle = 0;
int lineWidth		= 1;

static int spin = 0;


// forward
void LonLat2Point(float lon, float lat, Vector *pos);

void Resize( GLsizei iWidth, GLsizei iHeight ) 
{ 
	glViewport (0,0,iWidth,iHeight); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0,(GLfloat)iWidth/(GLfloat)iHeight, 0.1, 120.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Idle()
{
	//glutPostRedisplay();
}

void Init()
{
	earthTexture.LoadTGA("images/earth_vector.tga");
	// generate our sphere
    //int count = 0;
	for (int x=0; x<=EARTH_LON_RES; x++) {
		for (int y=0; y<=EARTH_LAT_RES; y++) {
            //count++;
			// angle around y-axis (which is x-value)
			float	angX, angY;
			angX = (x * 360.f / EARTH_LON_RES) * PI / 180.f;
			angY = (-90.f + (y * 180.f / EARTH_LAT_RES)) * PI / 180.f;
			vertices[x][y].x = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
			vertices[x][y].y = EARTH_RADIUS * sinf(angY);
			vertices[x][y].z = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
			mapping[x][y].u = (float)x / EARTH_LON_RES;
			mapping[x][y].v = (float)y / EARTH_LAT_RES;
            cout << angX << " - " << angY << endl;
		}
	}
    //cout << 'Count : ' << count;
}

void DrawEarth()
{	
	glEnable(GL_TEXTURE_2D);
	earthTexture.Use();	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glColor3f(1,1,1);
	int x, y;
	for (y=0; y<EARTH_LAT_RES; y++) {
		glBegin(GL_QUAD_STRIP);		
		for (x=0; x<EARTH_LON_RES; x++) {
            //glColor3d(1,1,1);
			glTexCoord2fv((float*)&mapping[x][y]);
			glVertex3fv((float*)&vertices[x][y]);
			glTexCoord2fv((float*)&mapping[x][y+1]);
			glVertex3fv((float*)&vertices[x][y+1]);
			glTexCoord2fv((float*)&mapping[x+1][y]);
			glVertex3fv((float*)&vertices[x+1][y]);
			glTexCoord2fv((float*)&mapping[x+1][y+1]);
			glVertex3fv((float*)&vertices[x+1][y+1]);            
		}
		glEnd();
	}	
    
    glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLineWidth(lineWidth);

		for (y=0; y<=EARTH_LAT_RES; y++) {
			glBegin(GL_LINE_STRIP);
			for (x=0; x<=EARTH_LON_RES; x++) {
				glColor4f(0,0,0,0.2f);
				glVertex3fv((float*)&vertices[x][y]);
			}
			glEnd();
		}

		for (x=0; x<=EARTH_LON_RES; x++) {
			glBegin(GL_LINE_STRIP);
			for (y=0; y<=EARTH_LAT_RES; y++) {
				glColor4f(0,0,0,0.2f);
				glVertex3fv((float*)&vertices[x][y]);
			}
			glEnd();
		}

		glDisable(GL_BLEND);
        
	glDisable(GL_TEXTURE_2D);
}

void LonLat2Point(float lon, float lat, Vector *pos)
{
	// lon -90..90
	// lat -180..180
	float	angX, angY;
	angX = (180.f+lat) * PI / 180.f;
	angY = lon * PI / 180.f;
	pos->x = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
	pos->y = EARTH_RADIUS * sinf(angY);
	pos->z = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
}

void DrawScene() 
{ 	  
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0,0,-125);
    glScalef(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE);
    //glScalef(scaleAll, scaleAll, scaleAll);
    glRotatef(rotY, 1,0,0);
    glRotatef(rotX, 0,1,0);
    GLdouble modelview_matrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
    DrawEarth();
    rotX++;
    glPopMatrix();
    GLfloat position[] = { 0.0, 0.0, 7000, 1.0 };

   
   glPushMatrix ();
   //gluLookAt (0.0, 0.0, -500.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   //glPushMatrix ();
   glRotated ((GLdouble) 90, 1.0, 0.0, 0.0);
   glLightfv (GL_LIGHT0, GL_POSITION, position);

   //glTranslated (0.0, 0.0, 1.5);
   //glDisable (GL_LIGHTING);
   //glColor3f (0.0, 1.0, 1.0);
   //glutWireCube (0.1);
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glPopMatrix ();
   
    glutSwapBuffers(); 
    //rotY++;glPushMatrix ();
    
}

void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            spin = (spin + 30) % 360;
            glutPostRedisplay();
         }
         break;
      default:
         break;
   }
}

int main(int argc, char **argv) 
{ 
	GLsizei iWidth = 640.0; 
	GLsizei iHeight = 480.0; 
	glutInit(&argc, argv);
	glutInitWindowPosition( 0, 0);
	glutInitWindowSize(iWidth, iHeight);
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
	glutCreateWindow( "Earth Simulation" );
	glutDisplayFunc(DrawScene );
	//glutIdleFunc(Idle );
	glutReshapeFunc(Resize );
	Init(); 
	//glClearColor(0.0f, 0.0f, 0.0f,0); 
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);
	Resize(iWidth, iHeight); 
    //glutMouseFunc(mouse);
	glutMainLoop(); 	
	return 0;
}
