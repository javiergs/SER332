#include "glut.h"

void display();
void resizeWindow(int newWidth, int newHeight);

int width = 500;
int height = 500;

float aspectRatio = 1;
float left, bottom = -1;
float right, top = 1;

float scaleFactor = 1;
float angle = 0;


void main(int argc, char ** argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Lab 01");

  // Callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(resizeWindow);
  glutMainLoop();

}

void resizeWindow(int newWidth, int newHeight) {

  aspectRatio = (float)newWidth / (float)newHeight;

  if (newWidth >= newHeight) {
    left = -1 * aspectRatio;
    right = aspectRatio;
    bottom = -1;
    top = 1;
  }
  else {
    left = -1;
    right = 1;
    bottom = -1 / aspectRatio;
    top = 1 / aspectRatio;
  }

  width = newWidth;
  height = newHeight;

}

void display() {

  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

  // Viewport 1
  glViewport(0, 0, width / 2, height / 2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(left, right, bottom, top);
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();

  glBegin(GL_QUADS);
  glColor3f(1, 0, 0); glVertex2f(.25, -.25);
  glColor3f(.5f, 0, .5f); glVertex2f(.25, .25);
  glColor3f(0, 0, 1); glVertex2f(-.25, .25);
  glColor3f(.5f, 0, .5f); glVertex2f(-.25, -.25);
  glEnd();

  glPopMatrix();

  glPushMatrix();

  glRotatef(angle, 0, 0, 1);
  glTranslatef(.9f, 0, 0);
  glRotatef(-angle, 0, 0, 1);

  glBegin(GL_QUADS);
  glColor3f(1, .5, 0); glVertex2f(.10, -.10);
  glColor3f(1, 0, 0); glVertex2f(.10, .10);
  glColor3f(1, .5, 0); glVertex2f(-.10, .10);
  glColor3f(1, 1, 0); glVertex2f(-.10, -.10);
  glEnd();
  glPopMatrix();

  scaleFactor += .0004;
  angle += .01;


  // Viewport 2
  glViewport(width / 2, height / 2, width / 2, height / 2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(left, right, bottom, top);

  float squareSize = .25;

  if (scaleFactor >= 2.5) { scaleFactor = 1; }

  glPushMatrix();

  glScalef(scaleFactor, scaleFactor, 0);

  glBegin(GL_QUADS);
  glColor3f(0, 1, 1);
  glVertex2f(squareSize, -squareSize);
  glVertex2f(squareSize, squareSize);
  glVertex2f(-squareSize, squareSize);
  glVertex2f(-squareSize, -squareSize);
  glEnd();

  glPopMatrix();


  // Viewport 3
  glViewport(0, height / 2, width / 2, height / 2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(left, right, bottom, top);

  glPushMatrix();

  glRotatef(angle, 0, 0, 1);

  glBegin(GL_QUADS);
  glColor3f(1, 1, 1); glVertex2f(.4, -.4);
  glColor3f(.8, 1, .8); glVertex2f(.4, .4);
  glColor3f(0, 1, 0); glVertex2f(-.4, .4);
  glColor3f(.8, 1, .8); glVertex2f(-.4, -.4);
  glEnd();

  glPopMatrix();


  glutPostRedisplay();
  glutSwapBuffers();

}
