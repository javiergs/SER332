#include "glut.h"
/*
 * Project 1
 * SER 332
 * February 2017
*/

/*
* GL callback function for display
*/
void myDisplayFunction();

/*
* GL callback function for special keys
*/
void mySpecialKeys(int key, int x, int y);

// variables used in calculating the location of vertices to allow moving the image
float objectOriginX;
float objectOriginY;

void main(int argc, char** argv)
{
  objectOriginX = 0.0f;
  objectOriginY = 0.0f;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Project 1");

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
  glClearColor(0.55f, 0.05f, 0.23f, 1.0f); // maroon
  glColor3f(0.99f, 0.78f, 0.15f); // gold

  glutSpecialFunc(mySpecialKeys);
  glutDisplayFunc(myDisplayFunction);
  glutMainLoop();
}

void myDisplayFunction()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // pitchfork
  glBegin(GL_LINE_LOOP);
  glVertex2f(objectOriginX + 0.00, objectOriginY - 0.60);
  glVertex2f(objectOriginX - 0.04, objectOriginY - 0.22);
  glVertex2f(objectOriginX - 0.03, objectOriginY - 0.22);
  glVertex2f(objectOriginX - 0.03, objectOriginY - 0.18);
  glVertex2f(objectOriginX - 0.04, objectOriginY - 0.18);
  glVertex2f(objectOriginX - 0.04, objectOriginY - 0.10);
  glVertex2f(objectOriginX - 0.12, objectOriginY - 0.04);
  glVertex2f(objectOriginX - 0.14, objectOriginY - 0.06);
  glVertex2f(objectOriginX - 0.26, objectOriginY + 0.38);
  glVertex2f(objectOriginX - 0.08, objectOriginY + 0.21);
  glVertex2f(objectOriginX - 0.16, objectOriginY + 0.26);
  glVertex2f(objectOriginX - 0.08, objectOriginY + 0.02);
  glVertex2f(objectOriginX - 0.04, objectOriginY + 0.00);
  glVertex2f(objectOriginX - 0.04, objectOriginY + 0.38);
  glVertex2f(objectOriginX - 0.08, objectOriginY + 0.36);
  glVertex2f(objectOriginX + 0.00, objectOriginY + 0.62);
  glVertex2f(objectOriginX + 0.08, objectOriginY + 0.36);
  glVertex2f(objectOriginX + 0.04, objectOriginY + 0.38);
  glVertex2f(objectOriginX + 0.04, objectOriginY + 0.00);
  glVertex2f(objectOriginX + 0.08, objectOriginY + 0.02);
  glVertex2f(objectOriginX + 0.16, objectOriginY + 0.26);
  glVertex2f(objectOriginX + 0.08, objectOriginY + 0.21);
  glVertex2f(objectOriginX + 0.26, objectOriginY + 0.38);
  glVertex2f(objectOriginX + 0.14, objectOriginY - 0.06);
  glVertex2f(objectOriginX + 0.12, objectOriginY - 0.04);
  glVertex2f(objectOriginX + 0.04, objectOriginY - 0.10);
  glVertex2f(objectOriginX + 0.04, objectOriginY - 0.18);
  glVertex2f(objectOriginX + 0.03, objectOriginY - 0.18);
  glVertex2f(objectOriginX + 0.03, objectOriginY - 0.22);
  glVertex2f(objectOriginX + 0.04, objectOriginY - 0.22);
  glEnd();

  // A
  glBegin(GL_LINE_LOOP);
  glVertex2f(objectOriginX + 0.30, objectOriginY + 0.30);
  glVertex2f(objectOriginX + 0.34, objectOriginY + 0.53);
  glVertex2f(objectOriginX + 0.33, objectOriginY + 0.54);
  glVertex2f(objectOriginX + 0.44, objectOriginY + 0.54);
  glVertex2f(objectOriginX + 0.43, objectOriginY + 0.53);
  glVertex2f(objectOriginX + 0.46, objectOriginY + 0.30);
  glVertex2f(objectOriginX + 0.41, objectOriginY + 0.30);
  glVertex2f(objectOriginX + 0.40, objectOriginY + 0.33);
  glVertex2f(objectOriginX + 0.36, objectOriginY + 0.33);
  glVertex2f(objectOriginX + 0.35, objectOriginY + 0.30);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex2f(objectOriginX + 0.37, objectOriginY + 0.38);
  glVertex2f(objectOriginX + 0.38, objectOriginY + 0.45);
  glVertex2f(objectOriginX + 0.39, objectOriginY + 0.38);
  glEnd();

  // S
  glBegin(GL_LINE_LOOP);
  glVertex2f(objectOriginX + 0.30, objectOriginY + 0.08);
  glVertex2f(objectOriginX + 0.30, objectOriginY + 0.02);
  glVertex2f(objectOriginX + 0.33, objectOriginY + 0.00);
  glVertex2f(objectOriginX + 0.43, objectOriginY + 0.00);
  glVertex2f(objectOriginX + 0.46, objectOriginY + 0.03);
  glVertex2f(objectOriginX + 0.46, objectOriginY + 0.09);
  glVertex2f(objectOriginX + 0.37, objectOriginY + 0.16);
  glVertex2f(objectOriginX + 0.37, objectOriginY + 0.19);
  glVertex2f(objectOriginX + 0.40, objectOriginY + 0.19);
  glVertex2f(objectOriginX + 0.40, objectOriginY + 0.16);
  glVertex2f(objectOriginX + 0.46, objectOriginY + 0.16);
  glVertex2f(objectOriginX + 0.46, objectOriginY + 0.21);
  glVertex2f(objectOriginX + 0.42, objectOriginY + 0.24);
  glVertex2f(objectOriginX + 0.35, objectOriginY + 0.24);
  glVertex2f(objectOriginX + 0.30, objectOriginY + 0.20);
  glVertex2f(objectOriginX + 0.30, objectOriginY + 0.16);
  glVertex2f(objectOriginX + 0.34, objectOriginY + 0.11);
  glVertex2f(objectOriginX + 0.40, objectOriginY + 0.07);
  glVertex2f(objectOriginX + 0.40, objectOriginY + 0.04);
  glVertex2f(objectOriginX + 0.36, objectOriginY + 0.04);
  glVertex2f(objectOriginX + 0.36, objectOriginY + 0.08);
  glEnd();

  // U
  glBegin(GL_LINE_LOOP);
  glVertex2f(objectOriginX + 0.30, objectOriginY - 0.06);
  glVertex2f(objectOriginX + 0.31, objectOriginY - 0.08);
  glVertex2f(objectOriginX + 0.31, objectOriginY - 0.26);
  glVertex2f(objectOriginX + 0.35, objectOriginY - 0.30);
  glVertex2f(objectOriginX + 0.43, objectOriginY - 0.30);
  glVertex2f(objectOriginX + 0.47, objectOriginY - 0.26);
  glVertex2f(objectOriginX + 0.47, objectOriginY - 0.08);
  glVertex2f(objectOriginX + 0.48, objectOriginY - 0.06);
  glVertex2f(objectOriginX + 0.40, objectOriginY - 0.06);
  glVertex2f(objectOriginX + 0.40, objectOriginY - 0.25);
  glVertex2f(objectOriginX + 0.38, objectOriginY - 0.25);
  glVertex2f(objectOriginX + 0.38, objectOriginY - 0.06);
  glEnd();

  glutSwapBuffers();
}

void mySpecialKeys(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    objectOriginY += 0.02f;
    break;
  case GLUT_KEY_LEFT:
    objectOriginX -= 0.02f;
    break;
  case GLUT_KEY_DOWN:
    objectOriginY -= 0.02f;
    break;
  case GLUT_KEY_RIGHT:
    objectOriginX += 0.02f;
    break;
  }
  glutPostRedisplay();
}
