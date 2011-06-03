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
// #include "gltexfont/gltexfont.h"

#include "countries/countries.h"


// menu
enum MenuCommands {
	MENU_TEXTURE	= 0,
	MENU_WIREFRAME,
	MENU_LINEWIDTH,
	MENU_COUNTRY_NAMES,	
	MENU_COAST,
	MENU_ISLAND,
	MENU_LAKE,
	MENU_RIVER,
	MENU_NATION,
	MENU_USSTATE,
	MENU_EXIT
};

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


// vector-file
struct VxpFeature {
	int		 nVerts;
	Vector	*pVerts;
};

struct Vxp {
	int			 nFeatures;
	VxpFeature	*pFeatures;
};


// variables
CGLTexture earthTexture;
  
Vector	vertices[EARTH_LON_RES+1][EARTH_LAT_RES+1];
Mapping mapping[EARTH_LON_RES+1][EARTH_LAT_RES+1];

Vector	country_names_pos[NUM_COUNTRIES];

int mouseX, mouseY;
int mouseButtonDown = 0, isCtrlDown = 0;

float rotX = 140, rotY = 0;
float autoRotX = 0, autoRotY = 0;
float scaleAll = 1;

int drawWireframe	= 1;
int drawTexture		= 1;
int showCountries	= 1;
int lineWidth		= 1;
int showVxp[6]		= {0,0,0,0,0,0};

Vxp	*vxp_data[6];

// forward
void LonLat2Point(float lon, float lat, Vector *pos);


// functions

// read binary-vector-data files (not a commercial format)
Vxp *load_bdv(char *pFileName)
{
	Vxp *vxp = new Vxp;

	FILE	*f;

	// read file
	f = fopen(pFileName, "rb");

	// read number of features
	fread(&vxp->nFeatures, 4, 1, f);
	// allocate features
	vxp->pFeatures = new VxpFeature[vxp->nFeatures];

	// start reading features
	for (int i=0; i< vxp->nFeatures; i++) {
		// read number of vertices
		fread(&vxp->pFeatures[i].nVerts, 4, 1, f);
		// allocate vertices
		vxp->pFeatures[i].pVerts = new Vector[ vxp->pFeatures[i].nVerts ];

		// scan vertices
		for (int j=0; j<vxp->pFeatures[i].nVerts; j++) {
			float lonlat[2];

			fread(lonlat, 8, 1, f);

			// project on sphere
			LonLat2Point(lonlat[1], lonlat[0], &vxp->pFeatures[i].pVerts[j]);
		}
	}

	fclose(f);

	return vxp;
}


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
	if (drawTexture) {
		glEnable(GL_TEXTURE_2D);
		earthTexture.Use();
	}

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

	if (drawWireframe) {

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

	static float time = 0;
	time += 0.1;

	// if the mouse-button is not down, auto-rotate
	if (!mouseButtonDown) {
		rotX += autoRotX*scaleAll*0.1f;
		rotY += autoRotY*scaleAll*0.1f;
	}

	glTranslatef(0,0,-125);
	glScalef(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE);
	glScalef(scaleAll, scaleAll, scaleAll);
	glRotatef(rotY, 1,0,0);
	glRotatef(rotX, 0,1,0);

	GLdouble modelview_matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);


	DrawEarth();

	// darw state vector-data
	glPushMatrix();
	glScalef(1.001f, 1.001f, 1.001f);
	glLineWidth(lineWidth);

	glPopMatrix();	
	glPopMatrix();
	glutSwapBuffers(); 
	//glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	int	deltX, deltY;

	// calculate mouse movement since click
	deltX = x - mouseX;
	deltY = y - mouseY;

	// store new mouse position
	mouseX = x;
	mouseY = y;

	if (mouseButtonDown) {
		// is Cotrl Down ?
		if (isCtrlDown) {
			// scale
			float addition;
			addition = ((deltX+deltY) / 200.f);
			
			if (addition < 0 && scaleAll+addition > MIN_SCALE) {
				scaleAll += addition;
			}

			if (addition > 0 && scaleAll+addition < MAX_SCALE) {
				scaleAll += addition;
			}
		} else {
			// rotate
			rotX += deltX*0.25f/scaleAll;
			rotY += deltY*0.25f/scaleAll;

			// save values for auto rotation
			autoRotX = deltX*0.25f;
			autoRotY = deltY*0.25f;
		}

		glutPostRedisplay();
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
