/*
 * LAB 1
 * http://javiergs.com/teaching/ser332
 */
 
#include "glut.h"

GLfloat angles = 0, scale = 1.5;
int defaultHeight = 500;
int defaultWidth = 500;
float aspectRatio;

void reshapeWindow(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspectRatio = (float)width / (float)height;
  if (width >= height)
  {
    gluOrtho2D(-defaultWidth*aspectRatio, defaultWidth*aspectRatio, -defaultHeight, defaultHeight);
  }
  else
  {
    gluOrtho2D(-defaultWidth, defaultWidth, -defaultHeight / aspectRatio, defaultHeight / aspectRatio);
  }
}

void init()
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glColor3f(0.0, 0.0, 0.0);
  glLoadIdentity();
}

void myIdle()
{
  if (angles > 360.0f)
  {
    angles = 0.0f;
  }
  angles += 0.1f;

  if (scale > 3.5f)
  {
    scale = 0.0f;
  }
  scale += 0.001f;

  glutPostRedisplay();
}

void display()
{
  // bottom left view port
  // square revolving around square
  glViewport(0, 0, 250, 250);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT);
  glShadeModel(GL_SMOOTH); // SMOOTH or FLAT
  glBegin(GL_QUADS);
  glColor3f(1.0, 1.0, 0.0); glVertex2f(0.25f, -0.25f);
  glColor3f(1.0, 0.647, 0.0); glVertex2f(0.25f, 0.25f);
  glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.25f, 0.25f);
  glColor3f(1.0, 0.647, 0.0); glVertex2f(-0.25f, -0.25f);
  glColor3f(1.0, 0.0, 0.0); glVertex2f(0.25f, -0.25f);
  glEnd();

  glRotatef(-angles, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.65, 0.0, 0.0);
  glRotatef(angles, 0.0f, 0.0f, 1.0f);

  glBegin(GL_QUADS);
  glColor3f(0.0f, 0.0f, 0.804f); glVertex2f(-0.05, 0.05);
  glVertex2f(-0.05, -0.05);
  glColor3f(0.502f, 0.0f, 0.0f); glVertex2f(0.05, -0.05);
  glVertex2f(0.05, 0.05);
  glEnd();

  // upper right viewport
  // scaling a square
  glViewport(250, 250, 250, 250);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glShadeModel(GL_SMOOTH); // SMOOTH or FLAT
  glScaled(scale, scale, scale);
  glBegin(GL_QUADS);
  glColor3f(0.753, 0.753, 0.753);
  glVertex2f(0.25f, -0.25f);
  glVertex2f(0.25f, 0.25f);
  glVertex2f(-0.25f, 0.25f);
  glVertex2f(-0.25f, -0.25f);
  glVertex2f(0.25f, -0.25f);
  glEnd();

  // upper right hand viewport
  // square spinning
  glViewport(0, 250, 250 * .5 * aspectRatio, 250 * .5*aspectRatio);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glShadeModel(GL_SMOOTH); // SMOOTH or FLAT
  glRotatef(angles, 0.0f, 0.0f, 1.0f);
  glBegin(GL_QUADS);
  glColor3f(0.000, 0.000, 1.000); glVertex2f(0.25f, -0.25f);
  glColor3f(0.000, 1.000, 1.000); glVertex2f(0.25f, 0.25f);
  glColor3f(0.000, 0.502, 0.000); glVertex2f(-0.25f, 0.25f);
  glColor3f(0.000, 1.000, 1.000); glVertex2f(-0.25f, -0.25f);
  glEnd();

  glutSwapBuffers(); // forces previously issued commands to execute
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(defaultWidth, defaultHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("In Class Lab");
  init();
  glutDisplayFunc(display);

  glutIdleFunc(myIdle);
  glutReshapeFunc(reshapeWindow);
  glutMainLoop();
  return 0;
}
