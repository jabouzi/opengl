// earth-simulation by Ohad Eder Pressman, 2001
// ohad@visual-i.com / ohad.visual-i.com

#define PI 3.14159265

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;
#include "GL/glut.h"
#include "GLTexture.h"


// defines
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
	glutPostRedisplay();
}

void Init()
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

void DrawEarth()
{	
	glEnable(GL_TEXTURE_2D);
	earthTexture.Use();	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glColor3f(1,1,1);
	int x, y;
	for (y=0; y<EARTH_LAT_RES; y++) {
		glBegin(GL_QUAD_STRIP);		
		for (x=0; x<EARTH_LON_RES; x++) {
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
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	glPushMatrix(); 
	glTranslatef(0,0,-125);
	glScalef(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE);
	glRotatef(angle,0,1,0);
	GLdouble modelview_matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
	DrawEarth();	
	glPopMatrix();
	glutSwapBuffers(); 
    angle ++;
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
	glutIdleFunc(Idle );
	glutReshapeFunc(Resize );
	Init(); 
	glClearColor(0.0f, 0.0f, 0.0f,0); 
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);
	Resize(iWidth, iHeight); 
	glutMainLoop(); 	
	return 0;
}
