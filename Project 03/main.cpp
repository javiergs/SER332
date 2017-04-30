/*
 * SER 332 Spring 2017
 * Project 3
 * March 2017
*/

#include "stdlib.h"
#include "glut.h"
#include "ImathVec.h"
#include "math.h"
#include <iostream>
#include <fstream>
using namespace std;

// WORLD TEXT FILE LOCATION
const char *worldFileName = "../world.txt";

// GLOBAL CONSTANTS
const int TYPE_BLOCK = 1;
const int TYPE_FLOOR = 0;
const int TYPE_TREE = 2;

const int WORLD_SIZE_X = 50;
const int WORLD_SIZE_Z = 50;

const float BLOCK_BASE = 10.0f;
const float BLOCK_RED_DURATION = 3.0f;
const float BLOCK_HEIGHT = 30.0f;

const float FLOOR_SIZE = 10.0f;

const float TREE_APOTHEM = 5.0f;
const float TREE_HEIGHT = 10.0f;
const float TREE_MAX_FLOAT = 20.0f;
const float TREE_MOVEMENT_PER_SECOND = 20.0f;

typedef Imath::Vec3<float> Vec3f;

// GLOBAL VARIABLES
int worldMap[WORLD_SIZE_X][WORLD_SIZE_Z];

GLuint wall_block;
float blockR = 0.0f;
float blockG = 0.0f;
float blockB = 1.0f;
float blockColorChangeTimeElapsed = 0.0f;

GLuint floor_square;
float floorR = 0.0f;
float floorG = 0.5f;
float floorB = 0.5f;

GLuint world_tree;
float treeR = 0.0f;
float treeG = 1.0f;
float treeB = 0.0f;
float treeY = 0.0f;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 1000.0f;

float cameraScale = 1.0f;
float cameraAngleX = 30.0f;
float cameraAngleY = 0.0f;

int mouseX;
int mouseY;
int mouseButton;

int screenWidth = 500;
int screenHeight = 500;
float screenRatio = 1;

int lastTimeStamp = 0;

// INIT FUNCTIONS
void init();
void initLists();
void initWorldMap();

// CALLBACK FUNCTIONS
void myDisplay();
void myIdleFunc();
void myKeyboardFunc(unsigned char key, int x, int y);
void myMouseFunc(int button, int state, int x, int y);
void myMotionFunc(int x, int y);
void myReshapeFunc(int x, int y);

// DRAW FUNCTIONS
void drawObject(int type, float x, float y, float z);

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Project 3");

  init();
  glutDisplayFunc(myDisplay);
  glutIdleFunc(myIdleFunc);
  glutKeyboardFunc(myKeyboardFunc);
  glutMouseFunc(myMouseFunc);
  glutMotionFunc(myMotionFunc);
  glutReshapeFunc(myReshapeFunc);
  glutMainLoop();
  return 0;
}

// basic initialization
void init()
{
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  initLists();
  initWorldMap();
}

// Initializes the display lists for the objects that can be in the world
void initLists()
{
  // Block list
  float halfBase = BLOCK_BASE / 2.0f;
  Vec3f b1 = Vec3f(-halfBase, 0.0f, halfBase);
  Vec3f b2 = Vec3f(halfBase, 0.0f, halfBase);
  Vec3f b3 = Vec3f(halfBase, 0.0f, -halfBase);
  Vec3f b4 = Vec3f(-halfBase, 0.0f, -halfBase);
  Vec3f b5 = Vec3f(-halfBase, BLOCK_HEIGHT, halfBase);
  Vec3f b6 = Vec3f(halfBase, BLOCK_HEIGHT, halfBase);
  Vec3f b7 = Vec3f(halfBase, BLOCK_HEIGHT, -halfBase);
  Vec3f b8 = Vec3f(-halfBase, BLOCK_HEIGHT, -halfBase);

  wall_block = glGenLists(1);
  glNewList(wall_block, GL_COMPILE);
  glBegin(GL_TRIANGLES);
  // bottom face
  glVertex3fv(&b2.x);
  glVertex3fv(&b1.x);
  glVertex3fv(&b3.x);

  glVertex3fv(&b4.x);
  glVertex3fv(&b3.x);
  glVertex3fv(&b1.x);

  // top face
  glVertex3fv(&b5.x);
  glVertex3fv(&b6.x);
  glVertex3fv(&b7.x);

  glVertex3fv(&b5.x);
  glVertex3fv(&b7.x);
  glVertex3fv(&b8.x);

  // front face
  glVertex3fv(&b1.x);
  glVertex3fv(&b2.x);
  glVertex3fv(&b5.x);

  glVertex3fv(&b5.x);
  glVertex3fv(&b2.x);
  glVertex3fv(&b6.x);

  // left face
  glVertex3fv(&b1.x);
  glVertex3fv(&b5.x);
  glVertex3fv(&b4.x);

  glVertex3fv(&b4.x);
  glVertex3fv(&b5.x);
  glVertex3fv(&b8.x);

  // right face
  glVertex3fv(&b2.x);
  glVertex3fv(&b3.x);
  glVertex3fv(&b6.x);

  glVertex3fv(&b6.x);
  glVertex3fv(&b3.x);
  glVertex3fv(&b7.x);

  // back face
  glVertex3fv(&b4.x);
  glVertex3fv(&b8.x);
  glVertex3fv(&b3.x);

  glVertex3fv(&b3.x);
  glVertex3fv(&b8.x);
  glVertex3fv(&b7.x);

  glEnd();
  glEndList();

  // Floor space list
  float halfSize = FLOOR_SIZE / 2.0f;
  Vec3f f1 = Vec3f(-halfSize, 0.0f, halfSize);
  Vec3f f2 = Vec3f(halfSize, 0.0f, halfSize);
  Vec3f f3 = Vec3f(halfSize, 0.0f, -halfSize);
  Vec3f f4 = Vec3f(-halfSize, 0.0f, -halfSize);

  floor_square = glGenLists(2);
  glNewList(floor_square, GL_COMPILE);
  glBegin(GL_TRIANGLES);
  // bottom face
  glVertex3fv(&f1.x);
  glVertex3fv(&f3.x);
  glVertex3fv(&f2.x);

  glVertex3fv(&f1.x);
  glVertex3fv(&f4.x);
  glVertex3fv(&f3.x);

  // top face
  glVertex3fv(&f1.x);
  glVertex3fv(&f2.x);
  glVertex3fv(&f3.x);

  glVertex3fv(&f1.x);
  glVertex3fv(&f3.x);
  glVertex3fv(&f4.x);

  glEnd();
  glEndList();

  // Tree list
  float radians = 6.28f; // a complete circle is 2 PI
                         // this will determine the distance from the center to a corner of the pentagon shaped base
  float hypotenuse = TREE_APOTHEM / cos(radians / 10.0f);
  Vec3f t1 = Vec3f(-hypotenuse*sin(0.0f), TREE_HEIGHT, -hypotenuse*cos(0.0f));
  Vec3f t2 = Vec3f(-hypotenuse*sin(radians / 5.0f), TREE_HEIGHT, -hypotenuse*cos(radians / 5.0f));
  Vec3f t3 = Vec3f(-hypotenuse*sin(2.0f*radians / 5.0f), TREE_HEIGHT, -hypotenuse*cos(2.0f*radians / 5.0f));
  Vec3f t4 = Vec3f(-hypotenuse*sin(3.0f*radians / 5.0f), TREE_HEIGHT, -hypotenuse*cos(3.0f*radians / 5.0f));
  Vec3f t5 = Vec3f(-hypotenuse*sin(4.0f*radians / 5.0f), TREE_HEIGHT, -hypotenuse*cos(4.0f*radians / 5.0f));
  Vec3f t6 = Vec3f(0.0f, 0.0f, 0.0f);

  world_tree = glGenLists(3);
  glNewList(world_tree, GL_COMPILE);
  glBegin(GL_TRIANGLES);

  // top face
  glVertex3fv(&t1.x);
  glVertex3fv(&t2.x);
  glVertex3fv(&t3.x);

  glVertex3fv(&t1.x);
  glVertex3fv(&t3.x);
  glVertex3fv(&t4.x);

  glVertex3fv(&t1.x);
  glVertex3fv(&t4.x);
  glVertex3fv(&t5.x);

  // front face
  glVertex3fv(&t6.x);
  glVertex3fv(&t4.x);
  glVertex3fv(&t3.x);

  // front right face
  glVertex3fv(&t6.x);
  glVertex3fv(&t5.x);
  glVertex3fv(&t4.x);

  // front left face
  glVertex3fv(&t6.x);
  glVertex3fv(&t3.x);
  glVertex3fv(&t2.x);

  // back right face
  glVertex3fv(&t6.x);
  glVertex3fv(&t1.x);
  glVertex3fv(&t5.x);

  // back left face
  glVertex3fv(&t6.x);
  glVertex3fv(&t2.x);
  glVertex3fv(&t1.x);

  glEnd();
  glEndList();
}

// Initialize the world map from the txt file set by worldFileName
void initWorldMap()
{
  ifstream in(worldFileName, ifstream::in);
  int *current = &worldMap[0][0];
  int count = 0;
  int maxCount = WORLD_SIZE_X * WORLD_SIZE_Z;

  if (in.fail())
  {
    cout << "Unable to open file " << worldFileName << endl;
    exit(1);
  }

  while (!in.eof() && count < maxCount)
  {
    char readChar = in.get();
    // ignore invalid characters
    if (readChar >= '0' && readChar <= '2')
    {
      // the numbers (0-9) start at 48 in ASCII
      int readInt = (int)readChar - 48;
      *current = readInt;
      current++;
      count++;
    }
  }
}

// Display the scene
void myDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, screenWidth, screenHeight);
  gluPerspective(45, screenRatio, 1, 3000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();

  gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glScalef(cameraScale, cameraScale, cameraScale);
  glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
  glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);

  float worldOriginX = -250.0f;
  float worldOriginZ = -250.0f;

  for (int i = 0; i<WORLD_SIZE_X; i++)
  {
    for (int j = 0; j<WORLD_SIZE_Z; j++)
    {
      drawObject(worldMap[i][j], (float)(i * 10) + worldOriginX, 0.0f, (float)(j * 10) + worldOriginZ);
    }
  }

  glPopMatrix();
  glutSwapBuffers();
}

// Handler for animating the scene
void myIdleFunc()
{
  int currentTimeStamp = glutGet(GLUT_ELAPSED_TIME);
  int elapsedTime = currentTimeStamp - lastTimeStamp;
  lastTimeStamp = currentTimeStamp;
  float seconds = (float)elapsedTime / 1000.0f;

  float movement = TREE_MOVEMENT_PER_SECOND * seconds;
  treeY += movement;

  while (treeY >= TREE_MAX_FLOAT * 2.0f)
  {
    treeY -= (TREE_MAX_FLOAT * 2.0f);
  }

  blockColorChangeTimeElapsed += seconds;

  if (blockColorChangeTimeElapsed > BLOCK_RED_DURATION)
  {
    blockR = 0.0f;
    blockG = 0.0f;
    blockB = 1.0f;
  }

  glutPostRedisplay();
}

// Handler for keyboard buttons
void myKeyboardFunc(unsigned char key, int x, int y)
{
  switch (key)
  {
  case ' ':
    blockR = 1.0f;
    blockG = 0.0f;
    blockB = 0.0f;

    blockColorChangeTimeElapsed = 0.0f;
    break;
  }
}

// Handler for mouse click
void myMouseFunc(int button, int state, int x, int y)
{
  mouseX = x;
  mouseY = y;
  mouseButton = button;
}

// Handler for mouse movement while a button is being pressed
void myMotionFunc(int x, int y)
{
  switch (mouseButton)
  {
  case GLUT_LEFT_BUTTON:
    cameraAngleX += (float(y - mouseY) / screenWidth) * 360.0f;
    cameraAngleY += (float(x - mouseX) / screenHeight) * 360.0f;

    while (cameraAngleX > 360.0f)
    {
      cameraAngleX -= 360.0f;
    }
    while (cameraAngleX < 0.0f)
    {
      cameraAngleX += 360.0f;
    }
    while (cameraAngleY > 360.0f)
    {
      cameraAngleY -= 360.0f;
    }
    while (cameraAngleY < 0.0f)
    {
      cameraAngleY += 360.0f;
    }

    break;

  case GLUT_RIGHT_BUTTON:
    cameraScale += (y - mouseY) / 100.0f;

    if (cameraScale < 0.1f)
    {
      cameraScale = 0.1f;
    }
    else if (cameraScale > 5.0f)
    {
      cameraScale = 5.0f;
    }

    break;
  }

  mouseX = x;
  mouseY = y;

  glutPostRedisplay();
}

// Handler for window resizing
void myReshapeFunc(int x, int y)
{
  screenWidth = x;
  screenHeight = y;

  if (screenHeight == 0)
  {
    screenHeight = 1;
  }

  screenRatio = (float)x / (float)y;
}

// Draws an object given the type in the location x, y, z
void drawObject(int type, float x, float y, float z)
{
  glPushMatrix();
  glTranslatef(x, y, z);

  switch (type)
  {
  case TYPE_BLOCK:
    glColor3f(blockR, blockG, blockB);
    glCallList(wall_block);
    break;
  case TYPE_TREE:
    glColor3f(treeR, treeG, treeB);
    glPushMatrix();
    if (treeY > TREE_MAX_FLOAT)
    {
      glTranslatef(0.0f, TREE_MAX_FLOAT * 2.0f - treeY, 0.0f);
    }
    else
    {
      glTranslatef(0.0f, treeY, 0.0f);
    }
    glCallList(world_tree);
    glPopMatrix();
    // break intentionally left out to continue to the next case as the tree must be drawn on a floor tile
  case TYPE_FLOOR:
    glColor3f(floorR, floorG, floorB);
    glCallList(floor_square);
    break;
  }

  glPopMatrix();
}
