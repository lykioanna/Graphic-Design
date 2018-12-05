#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <istream>
#include <iostream>

#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "visuals.h"   // Header file for our OpenGL functions

using namespace std;


model md;
float opacity = 1.0;
bool animation = true;
bool opacity_flag = true;
int flag = 1;
static float roty = 0.0;
static float tx = 0.0;
static float ty = 0.0;

void Render()
{    
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(tx, 0.1, 0.0, 0.0);
	glRotatef(ty, 0.0, 0.1, 0.0);

//asteria
	int b;
	static int stars[100][3];
	while (flag)
	{
		// Initialize spheres with random x,y,z coordinates.
		for (b = 0; b < 99; b++) {
			stars[b][0] = int(rand() % 150 - 100);
			stars[b][1] = int(rand() % 150 - 100);
			stars[b][2] = int(rand() % 150 - 100);
		}
		flag = 0;
	}
	for (b = 0; b < 99; b++) {    // Translate balls towards and away from front plane.
		glPushMatrix();
		glTranslatef(stars[b][0], stars[b][1], stars[b][2]);
		glColor3f(1.0, 1.0, 1.0);
		glutSolidSphere(0.1, 20, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(stars[b][0], stars[b][1], stars[b][2]);
		glColor4f(1.0, 1.0, 1.0, opacity);
		glutSolidSphere(0.3, 20, 50);

		glPopMatrix();
	}

//Hlios
	glPushMatrix();
	glTranslatef(0, 0.0, 0.0);
	glColor3f(1.5, 0.8, 0.0);
	glutSolidSphere(13.0, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.0, 0.0);
	glColor4f(1.0, 0.9, 0.0, opacity);
	glutSolidSphere(17.0, 18, 18);
	glPopMatrix();
	
	glPushMatrix();
//planitis
	glRotatef(roty, 0.0, 1.0, 0.0);
	glPushMatrix();


	glTranslatef(40, 0, 0);
	glScalef(0.012, 0.012, 0.012);
	glRotatef(roty, 0.0, 1.0, 0.0);
	glColor3f(3.0, 0.0, 0.0);                            // Set drawing colour
	DisplayModel(md);

	glPopMatrix();

//doriforos
	glTranslatef(40, 0, 0);

	glRotatef(roty, 0.0, 1.0, 0.0);
	glTranslatef(-10, 0, 0);
	glScalef(0.003, 0.003, 0.003);
	glColor3f(0.0, 0.0, 3.0);                            // Set drawing colour
	DisplayModel(md);

	glPopMatrix();
	glutSwapBuffers();
}

//-----------------------------------------------------------


void Idle()
{

	//kinhsh asteriwn/hlioy
	if (animation) {
		
		roty += 2.0;
		if (opacity_flag) {
			opacity -= 0.01;
			if (opacity <= 0.0) opacity_flag = false;
		}
		else {
			opacity += 0.01;
			if (opacity >= 1.0) opacity_flag = true;
		}
	}
	glutPostRedisplay();
}

//....

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's': ty += 0.5f;
		break;
	case 'w': ty -= 0.5f;
		break;
	case 'a': tx += 0.5f;
		break;
	case 'd': tx -= 0.5f;
		break;
	default: break;
	}

	glutPostRedisplay();

}
//....

void Resize(int w, int h)
{ 
	// define the visible area of the window ( in pixels )
	if (h==0) h=1;
	glViewport(0,0,w,h); 

	// Setup viewing volume

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	 
	          // L     R       B      T      N      F
	glOrtho (-50.0f, 50.0f, -50.0f, 50.0f,-500.0f,500.0f);

}


//-----------------------------------------------------------

void Setup()  
{ 
	ReadFile(&md);

   glEnable( GL_CULL_FACE );

   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc( GL_LEQUAL );      
   glClearDepth(1.0);	      


   //Set up light source
   GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
   GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
   GLfloat lightPos[] = { -20.0, 20.0, 150.0, 1.0 };

   glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
   glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
   glLightfv( GL_LIGHT0, GL_POSITION,lightPos );


 
	// polygon rendering mode and material properties
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
   
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0);

	// BLENDING
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
}


void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		animation = !animation;
		glutPostRedisplay();
	}
}

void ReadFile(model *md)
{

	ifstream obj_file("planet.obj");

	if (obj_file.fail())
		exit(1);

	md->vertices = 0;
	md->faces = 0;
	md->normals = 0;

	string line;

	while (getline(obj_file, line)) {

		if (line.length() > 0) {	// ignore empty lines	

			if ((line.at(0) == 'v')) {	// 'v' or 'vn' - vertexes and normals
				if (line.at(1) == ' ') {	// 'v'
					istringstream stm(line);
					stm.ignore(1, ' ');	// ignore 'v'
					stm >> md->obj_points[md->vertices].x;
					stm >> md->obj_points[md->vertices].y;
					stm >> md->obj_points[md->vertices].z;
					md->vertices++;
				}
				else if (line.at(1) == 'n') {	// 'vn'
					istringstream stm(line);
					stm.ignore(2, ' ');	// ignore 'vn'
					stm >> md->obj_normals[md->normals].x;
					stm >> md->obj_normals[md->normals].y;
					stm >> md->obj_normals[md->normals].z;
					md->normals++;
				}
			}
			else if (line.at(0) == 'f') {	// 'f' - faces	
											// Split line to its components: v1//n1 v2//n2 v3//n3
				int pos;
				pos = line.find("//");
				line.replace(pos, 2, "  ");
				pos = line.find("//", pos + 1);
				line.replace(pos, 2, "  ");
				pos = line.find("//", pos + 1);
				line.replace(pos, 2, "  ");
				// Create a string stream
				istringstream stm(line);
				// ignore 'f'
				stm.ignore(1, ' ');
				stm >> md->obj_faces[md->faces].vtx[0];
				stm >> md->obj_faces[md->faces].norm[0];
				stm >> md->obj_faces[md->faces].vtx[1];
				stm >> md->obj_faces[md->faces].norm[1];
				stm >> md->obj_faces[md->faces].vtx[2];
				stm >> md->obj_faces[md->faces].norm[2];
				md->faces++;
			}
		}
	}
	obj_file.close();
}

void DisplayModel(model md)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
	{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0] - 1].x, md.obj_points[md.obj_faces[i].vtx[0] - 1].y, md.obj_points[md.obj_faces[i].vtx[0] - 1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1] - 1].x, md.obj_points[md.obj_faces[i].vtx[1] - 1].y, md.obj_points[md.obj_faces[i].vtx[1] - 1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2] - 1].x, md.obj_points[md.obj_faces[i].vtx[2] - 1].y, md.obj_points[md.obj_faces[i].vtx[2] - 1].z);
	}

	glEnd();
	glPopMatrix();

}




