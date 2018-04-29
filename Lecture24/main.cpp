/*
* Lecture 24
* http://javiergs.com/teaching/ser332
*
* Coding review
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include "glut.h"
#include<iostream>
#include<string>
#include <windows.h>
#include <wingdi.h>

using namespace std;

// GLOBAL
int WIDTH = 1200, HEIGHT= 600;

// TEXTURE
GLuint texture_from_algorithm() {
	GLuint texture;
	const int TexHeight = 128;
	const int TexWidth = 128;
	GLubyte textureImage[TexHeight][TexWidth][3];
	for (int i = 0; i < TexHeight; i++)
		for (int j = 0; j < TexWidth; j++) {
			textureImage[i][j][0] = 127 + i;	 
			textureImage[i][j][1] = 0;				 
			textureImage[i][j][2] = 127 + j;	
		}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TexWidth, TexHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, textureImage);
	return texture;
}

// DISPLAY
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIDTH, HEIGHT);
	gluPerspective(45, 1, 1, 1000);
	// VIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// LOOKAT
	gluLookAt(0.0f, 40.0f, 320.0,
						0.0f, 1.0f, -1.0f,
						0.0f, 1.0f, 0.0f);
	// SQUARE
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture_from_algorithm());
  glBegin(GL_TRIANGLES);
	  glVertex3f(0.0, 50.0, 50.0);
		glNormal3f(0.0,  0.0,  1.0);		
		glVertex3f(50.0,50.0, 50.0);
		glNormal3f( 0.0, 0.0,  1.0);		
		glVertex3f(50.0, 0.0, 50.0);		
		glNormal3f( 0.0, 0.0,  1.0);
		glVertex3f(50.0, 0.0, 50.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 50.0);
		glNormal3f(0.0, 0.0,  1.0);		
		glVertex3f(0.0, 50.0, 50.0);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glTexCoord2f(1.0, 1.0);
	  glTexCoord2f(1.0, 0.0);
		glTexCoord2f(0.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// SWAP
	glutSwapBuffers();
}

// MAIN
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Coding review");	
	// CALLBACK
	glutDisplayFunc(display);	
	// SHADE AND DEPTH
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	// LIGHT
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	// LOOP
	glutMainLoop();
}
