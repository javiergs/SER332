#include "glut.h"
/*
 * Project 2
 * SER 332
 * February 2017
*/

const int MOON_ORBITAL_PERIOD = 24;
const int PLANET_ORBITAL_PERIOD = 360;

float orthoX;
float orthoY;
int viewportSizeX;
int viewportSizeY;

int moonRotation;
int planetRotation;

float carTranslationX;
int wheelRotation;

int hourRotation;
int minuteRotation;

float asuTranslationX;
float asuTranslationY;

/*
* GL callback function for display.
*/
void myDisplayFunction();

/*
* Initializes a new viewport with the given dimensions.
*/
void initViewport(int x, int y, int width, int height);

/*
* GL callback function for idle
*/
void update();

void windowResize(int x, int y);

/*
* Draw a viewport with a moon orbiting a planet orbitinga star.
*/
void drawViewportOrbit();

/*
* Draw a viewport with a car moving from right to left and back to the right.
*/
void drawViewportCar();

/*
* Draw a viewport with an analog clock.
*/
void drawViewportClock();

/*
* Draw a viewport containing the ASU pitchfork.
*/
void drawViewportASU();

/*
* GL callback function for special keys
*/
void mySpecialKeys(int key, int x, int y);

int main(int argc, char** argv)
{
  int windowSizeX = 500;
  int windowSizeY = 500;
  viewportSizeX = windowSizeX / 2;
  viewportSizeY = windowSizeY / 2;
  orthoX = 1.0f;
  orthoY = 1.0f;

  moonRotation = 0;
  planetRotation = 0;

  carTranslationX = 1.0f;
  wheelRotation = 0;

  hourRotation = 0;
  minuteRotation = 0;

  asuTranslationX = 0.0f;
  asuTranslationY = 0.0f;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(windowSizeX, windowSizeY);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("James Smith Project 2");

  glutDisplayFunc(myDisplayFunction);
  glutIdleFunc(update);
  glutReshapeFunc(windowResize);
  glutSpecialFunc(mySpecialKeys);
  glutMainLoop();

  return 0;
}

void myDisplayFunction()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black
  glClear(GL_COLOR_BUFFER_BIT);

  initViewport(0, viewportSizeY, viewportSizeX, viewportSizeY);
  drawViewportOrbit();

  initViewport(viewportSizeX, viewportSizeY, viewportSizeX, viewportSizeY);
  drawViewportCar();

  initViewport(0, 0, viewportSizeX, viewportSizeY);
  drawViewportClock();

  initViewport(viewportSizeX, 0, viewportSizeX, viewportSizeY);
  drawViewportASU();

  glutSwapBuffers();
}

void initViewport(int x, int y, int width, int height)
{
  glViewport(x, y, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-orthoX, orthoX, -orthoY, orthoY);
}

void update()
{
  int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
  int orbitPosition = elapsedTime / 50;
  float carPosition = (float)(elapsedTime % 8000) / 200.0f;
  bool carMovingLeft = (carPosition) < 20;
  int clockTime = (elapsedTime / 10) % (360 * 12);

  moonRotation = (360 * orbitPosition / MOON_ORBITAL_PERIOD) % 360;
  planetRotation = (360 * orbitPosition / PLANET_ORBITAL_PERIOD) % 360;

  if (carMovingLeft)
  {
    carTranslationX = (-carPosition + 10.0f) / 10.0f;
  }
  else
  {
    carTranslationX = (carPosition - 30.0f) / 10.0f;
  }
  // wheel rotation = (movement / wheel circumference) * 360 degrees
  wheelRotation = -(int)(carTranslationX / 0.222f * 360.0f) % 360;

  minuteRotation = -(clockTime % 360);
  hourRotation = -(clockTime / 12);

  glutPostRedisplay();
}

void windowResize(int x, int y)
{
  float aspectRatio = (float)x / (float)y;
  viewportSizeX = x / 2;
  viewportSizeY = y / 2;

  if (x >= y)
  {
    orthoX = aspectRatio;
    orthoY = 1.0f;
  }
  else
  {
    orthoX = 1.0f;
    orthoY = 1.0f / aspectRatio;
  }

  glutPostRedisplay();
}

void drawViewportOrbit()
{
  // star
  glBegin(GL_QUADS);
  glColor3f(0.99f, 0.78f, 0.15f); // yellow
  glVertex2f(-0.1, -0.1);
  glVertex2f(0.1, -0.1);
  glVertex2f(0.1, 0.1);
  glVertex2f(-0.1, 0.1);
  glEnd();

  glPushMatrix();

  glRotatef(planetRotation, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.5f, 0.0f, 0.0f);
  // planet
  glBegin(GL_QUADS);
  glColor3f(0.0f, 0.0f, 1.00f); // blue
  glVertex2f(-0.04, -0.04);
  glVertex2f(0.04, -0.04);
  glVertex2f(0.04, 0.04);
  glVertex2f(-0.04, 0.04);
  glEnd();

  glRotatef(moonRotation, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.1f, 0.0f, 0.0f);
  // moon
  glBegin(GL_QUADS);
  glColor3f(0.9f, 0.9f, 0.9f); // gray
  glVertex2f(-0.02, -0.02);
  glVertex2f(0.02, -0.02);
  glVertex2f(0.02, 0.02);
  glVertex2f(-0.02, 0.02);
  glEnd();

  glPopMatrix();
}

void drawViewportCar()
{
  glPushMatrix();

  glTranslatef(carTranslationX, 0.0f, 0.0f);

  // body
  glBegin(GL_QUADS);
  glColor3f(1.0f, 0.0f, 0.0f); // red
  glVertex2f(-0.2, -0.06);
  glVertex2f(0.2, -0.06);
  glVertex2f(0.2, 0.06);
  glVertex2f(-0.2, 0.06);
  glEnd();

  glPushMatrix();
  glTranslatef(-0.15, -.0475, 0.0);
  glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);

  // left wheel
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f); // white
  glVertex2f(-0.025, -0.025);
  glVertex2f(0.025, -0.025);
  glVertex2f(0.025, 0.025);
  glVertex2f(-0.025, 0.025);
  glEnd();

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.15, -.0475, 0.0);
  glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);

  // right wheel
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f); // white
  glVertex2f(-0.025, -0.025);
  glVertex2f(0.025, -0.025);
  glVertex2f(0.025, 0.025);
  glVertex2f(-0.025, 0.025);
  glEnd();

  glPopMatrix();
  glPopMatrix();
}

void drawViewportClock()
{
  glPushMatrix();
  glRotatef(hourRotation, 0.0f, 0.0f, 1.0f);

  // hour hand
  glBegin(GL_QUADS);
  glColor3f(0.55f, 0.05f, 0.23f); // maroon
  glVertex2f(-0.08, 0.0);
  glVertex2f(0.08, 0.0);
  glVertex2f(0.08, 0.5);
  glVertex2f(-0.08, 0.5);
  glEnd();

  glPopMatrix();

  glPushMatrix();
  glRotatef(minuteRotation, 0.0f, 0.0f, 1.0f);

  // minute hand
  glBegin(GL_QUADS);
  glColor3f(0.99f, 0.78f, 0.15f); // gold
  glVertex2f(-0.05, 0.0);
  glVertex2f(0.05, 0.0);
  glVertex2f(0.05, 0.8);
  glVertex2f(-0.05, 0.8);
  glEnd();

  glPopMatrix();
}

void drawViewportASU()
{
  glPushMatrix();
  glTranslatef(asuTranslationX, asuTranslationY, 0.0f);

  glColor3f(0.99f, 0.78f, 0.15f); // gold

                                  // pitchfork
  glBegin(GL_LINE_LOOP);
  glVertex2f(0.00, -0.60);
  glVertex2f(-0.04, -0.22);
  glVertex2f(-0.03, -0.22);
  glVertex2f(-0.03, -0.18);
  glVertex2f(-0.04, -0.18);
  glVertex2f(-0.04, -0.10);
  glVertex2f(-0.12, -0.04);
  glVertex2f(-0.14, -0.06);
  glVertex2f(-0.26, 0.38);
  glVertex2f(-0.08, 0.21);
  glVertex2f(-0.16, 0.26);
  glVertex2f(-0.08, 0.02);
  glVertex2f(-0.04, 0.00);
  glVertex2f(-0.04, 0.38);
  glVertex2f(-0.08, 0.36);
  glVertex2f(0.00, 0.62);
  glVertex2f(0.08, 0.36);
  glVertex2f(0.04, 0.38);
  glVertex2f(0.04, 0.00);
  glVertex2f(0.08, 0.02);
  glVertex2f(0.16, 0.26);
  glVertex2f(0.08, 0.21);
  glVertex2f(0.26, 0.38);
  glVertex2f(0.14, -0.06);
  glVertex2f(0.12, -0.04);
  glVertex2f(0.04, -0.10);
  glVertex2f(0.04, -0.18);
  glVertex2f(0.03, -0.18);
  glVertex2f(0.03, -0.22);
  glVertex2f(0.04, -0.22);
  glEnd();

  // A
  glBegin(GL_LINE_LOOP);
  glVertex2f(0.30, 0.30);
  glVertex2f(0.34, 0.53);
  glVertex2f(0.33, 0.54);
  glVertex2f(0.44, 0.54);
  glVertex2f(0.43, 0.53);
  glVertex2f(0.46, 0.30);
  glVertex2f(0.41, 0.30);
  glVertex2f(0.40, 0.33);
  glVertex2f(0.36, 0.33);
  glVertex2f(0.35, 0.30);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex2f(0.37, 0.38);
  glVertex2f(0.38, 0.45);
  glVertex2f(0.39, 0.38);
  glEnd();

  // S
  glBegin(GL_LINE_LOOP);
  glVertex2f(0.30, 0.08);
  glVertex2f(0.30, 0.02);
  glVertex2f(0.33, 0.00);
  glVertex2f(0.43, 0.00);
  glVertex2f(0.46, 0.03);
  glVertex2f(0.46, 0.09);
  glVertex2f(0.37, 0.16);
  glVertex2f(0.37, 0.19);
  glVertex2f(0.40, 0.19);
  glVertex2f(0.40, 0.16);
  glVertex2f(0.46, 0.16);
  glVertex2f(0.46, 0.21);
  glVertex2f(0.42, 0.24);
  glVertex2f(0.35, 0.24);
  glVertex2f(0.30, 0.20);
  glVertex2f(0.30, 0.16);
  glVertex2f(0.34, 0.11);
  glVertex2f(0.40, 0.07);
  glVertex2f(0.40, 0.04);
  glVertex2f(0.36, 0.04);
  glVertex2f(0.36, 0.08);
  glEnd();

  // U
  glBegin(GL_LINE_LOOP);
  glVertex2f(0.30, -0.06);
  glVertex2f(0.31, -0.08);
  glVertex2f(0.31, -0.26);
  glVertex2f(0.35, -0.30);
  glVertex2f(0.43, -0.30);
  glVertex2f(0.47, -0.26);
  glVertex2f(0.47, -0.08);
  glVertex2f(0.48, -0.06);
  glVertex2f(0.40, -0.06);
  glVertex2f(0.40, -0.25);
  glVertex2f(0.38, -0.25);
  glVertex2f(0.38, -0.06);
  glEnd();

  glPopMatrix();
}

void mySpecialKeys(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    asuTranslationY += 0.02f;
    break;
  case GLUT_KEY_LEFT:
    asuTranslationX -= 0.02f;
    break;
  case GLUT_KEY_DOWN:
    asuTranslationY -= 0.02f;
    break;
  case GLUT_KEY_RIGHT:
    asuTranslationX += 0.02f;
    break;
  }
  glutPostRedisplay();
}
