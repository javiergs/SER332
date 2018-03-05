/**
 * Lecture 05: Vertex and Shading
 * http://javiergs.com/teaching/ser332
 */

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
 * myInit
 */
void myInit() {
  glClearColor(0.764f, 0.129f, 0.282f, 1.0); // maroon clear color
  glMatrixMode(GL_PROJECTION); // projection transformations
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // units inside
}

/**
 * myDisplay
 */
void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT); // clear the window
  glShadeModel(GL_FLAT);  //SMOOTH or FLAT
  glBegin(GL_POLYGON);  // fill connected polygon
  glColor3f(1.0f, 0.843f, 0.0f); glVertex2f(-0.7, 0.7); // vertices of the triangl 1
  glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(0.6, 0.7);
  glColor3f(0.0f, 0.0f, 0.0f); glVertex2f(-0.7, -0.6);
  glEnd();
  glShadeModel(GL_SMOOTH); //SMOOTH or FLAT
  glBegin(GL_POLYGON);  // fill connected polygon
  glColor3f(1.0f, 0.843f, 0.0f); glVertex2f(0.7, 0.6); // vertices of the triangle 2
  glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(-0.6, -0.7);
  glColor3f(0.0f, 0.0f, 0.0f); glVertex2f(0.7, -0.7);
  glEnd();
  glFlush(); //forces issued commands to execute
}

/**
 * main
 */
void main(int argc, char** argv) {
  glutInit(&argc, argv); // glut init
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500); // actual window size	
  glutInitWindowPosition(0, 0); // window location
  glutCreateWindow("Lecture 05");
  myInit();
  glutDisplayFunc(myDisplay);
  glutMainLoop(); //event loop 
}
