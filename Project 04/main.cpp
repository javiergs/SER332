/*
 * SER 332 Spring 2017
 * Project 4
 * April 2017
*/

#include <vector>
#include "imathvec.h"
#include "glut.h"
#include<iostream>
#include<fstream>
#include<string>
#include <windows.h>
#include <wingdi.h>
#define PI 3.1415926
using namespace std;
using namespace Imath;

typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;

struct Mesh {
  // vertex
  vector<Vec3f> dot_vertex;
  vector<Vec3f> dot_normalPerFace;
  vector<Vec3f> dot_normalPerVertex;
  vector<Vec2f> dot_texture;
  // faces
  vector<int> face_index_vertex;
  vector<int> face_index_normalPerFace;
  vector<int> face_index_normalPerVertex;
  vector<int> face_index_texture;
};

// NOISE

int permutation[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

// noise class
class ImprovedNoise {

private:
  int p[512];

public:

  ImprovedNoise() {
    for (int i = 0; i < 256; i++)
      p[256 + i] = p[i] = permutation[i];
  }

public:

  double perlinMarble(float x, float y) {
    double total = 0;
    double scale = 0.005;
    double p = 2.0; //0.1; //persistence 
    double n = 4.0; // number of octaves
    double frequency, amplitude;
    for (int i = 0; i<n; i++) {
      frequency = pow(2, (double)i);
      amplitude = pow(p, (double)-i) * 2;
      total = total + noise(scale*frequency* x, scale*frequency* y, 11.5) * amplitude;
    }
    return sin(2 * (x*scale) + total);
  }

  double perlinMultiscale(float x, float y) {
    double total = 0;
    double scale = 0.005;
    double p = 2.0; //0.1; //persistence 
    double n = 3.0; // number of octaves
    double frequency, amplitude;
    for (int i = 0; i<n; i++) {
      frequency = pow(2, (double)i);
      amplitude = pow(p, (double)-i) * 2;
      total = total + noise(scale*frequency* x, scale*frequency* y, 11.5) * amplitude;
    }
    return total;
  }

  double noise(double x, double y, double z) {
    // find unit cube that contains point
    int X = (int)floor(x) & 255;
    int	Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;
    // find relative x,y,z of point in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    //compute fade curves for each x,y,z
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);
    // hash coordinates of the 8 cube corners
    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
    int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;
    // add blended results from 8 corners of cube
    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
      grad(p[BA], x - 1, y, z)),
      lerp(u, grad(p[AB], x, y - 1, z),
        grad(p[BB], x - 1, y - 1, z))),
      lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
        grad(p[BA + 1], x - 1, y, z - 1)),
        lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
          grad(p[BB + 1], x - 1, y - 1, z - 1))));
  }

  static double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
  }

  static double lerp(double t, double a, double b) {
    return a + t * (b - a);
  }

  // convert 4 bits of hash code into 12 gradient directions.
  static double grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h<8 ? x : y, v = h<4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
  }

} iNoise;

// NAVIGATION
const int GRID_SIZE = 20;
int nav_map[50][50] = { { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };

// WINDOW SETTINGS

int screen_width = 600;
int screen_height = 600;
float screen_ratio = 1;

int last_time_stamp = 0;

// WORLD OBJECTS
GLuint hallway_list;
GLuint room1_list;
GLuint room2_list;
GLuint room3_list;
GLuint room4_list;

GLuint bed_list;
GLuint bookshelf_list;
GLuint ceiling_light_list;
GLuint desk_list;
GLuint desk_light_list;
GLuint floor_lamp_list;
GLuint fluorescent_light_list;
GLuint hanging_light_list;
GLuint sink_list;
GLuint table_list;
GLuint table_chair_list;

const float HANGING_LIGHT_PERIOD = 0.8f;
const float HANGING_LIGHT_RANGE = 20.0f;
float hanging_light_angle = 0.0f;

// PLAYER
float player_x = 0.0f;
float player_y = 50.0f;
float player_z = 300.0f;
float player_angle = PI;
float player_move_speed = 20.0f;
float player_rotate_speed = PI / 20.0f;

// INIT FUNCTIONS
void init();
void initHallway();
void initRoom1();
void initRoom2();
void initRoom3();
void initRoom4();
void initWorld();

// CALLBACK FUNCTIONS
void myDisplay();
void myIdleFunc();
void myReshapeFunc(int x, int y);
void mySpecialFunc(int key, int x, int y);

// HELPER FUNCTIONS
int StrToInt(const string &str);
vector<string> split_string(const string& str, const string& split_str);
Mesh* loadFile(const char* file);
void calculateNormalPerFace(Mesh* m);
void calculateNormalPerVertex(Mesh* m);
Mesh* createPlane(int arena_width, int arena_depth, int arena_cell);
GLuint meshToDisplayList(Mesh* m, int id);
void setLighting(int x, int y);
float getHangingLightAngle();

// TEXTURES
const int MULTISCALE_BLUE = 0;
const int MULTISCALE_WHITE = 2;
const int MULTISCALE_RED = 3;
const int MULTISCALE_GREEN = 4;
const int MARBLE = 1;

GLuint texture_array[16];

GLubyte *LoadDIBitmap(const char *filename, BITMAPINFO **info);
void bmpTexture(UINT textureArray[], LPSTR file, int n);
void codedTexture(UINT textureArray[], int n, int type, float amp, float frequency, float phase);

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(screen_width, screen_height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Project 4");

  glutDisplayFunc(myDisplay);
  glutIdleFunc(myIdleFunc);
  glutReshapeFunc(myReshapeFunc);
  glutSpecialFunc(mySpecialFunc);

  init();

  glutMainLoop();
  return 0;
}

void init()
{
  glShadeModel(GL_SMOOTH);

  // TEXTURES
  bmpTexture(texture_array, "../bmp files/p_stones.bmp", 0);
  bmpTexture(texture_array, "../bmp files/p_floor.bmp", 1);
  bmpTexture(texture_array, "../bmp files/p_bricks.bmp", 2);
  bmpTexture(texture_array, "../bmp files/p_granite.bmp", 3);
  bmpTexture(texture_array, "../bmp files/p_darkwood.bmp", 4);
  codedTexture(texture_array, 5, MULTISCALE_WHITE, 2.0f, 12.0f, 0.0f);
  bmpTexture(texture_array, "../bmp files/p_woodlight.bmp", 6);
  codedTexture(texture_array, 7, MULTISCALE_BLUE, 1.0f, 4.0f, 0.0f);
  codedTexture(texture_array, 8, MARBLE, 1.0f, 15.0f, 0.0f);
  codedTexture(texture_array, 9, MULTISCALE_GREEN, 2.0f, 12.0f, 0.0f);
  codedTexture(texture_array, 10, MULTISCALE_RED, 2.0f, 12.0f, 0.0f);
  bmpTexture(texture_array, "../bmp files/p_qubodup.bmp", 11);
  bmpTexture(texture_array, "../bmp files/p_carpet.bmp", 12);
  bmpTexture(texture_array, "../bmp files/p_tile.bmp", 13);

  Mesh *bed = loadFile("../obj files/p_bed.obj");
  Mesh *bookshelf = loadFile("../obj files/p_bookcase.obj");
  Mesh *ceiling_light = loadFile("../obj files/p_bubble.obj");
  Mesh *desk = loadFile("../obj files/p_table.obj");
  Mesh *desk_light = loadFile("../obj files/p_lamp.obj");
  Mesh *floor_lamp = loadFile("../obj files/p_livingroom_lamp.obj");
  Mesh *fluorescent_light = loadFile("../obj files/p_fluorescent.obj");
  Mesh *hanging_light = loadFile("../obj files/p_hanging-restaurant-light.obj");
  Mesh *sink = loadFile("../obj files/p_cabinetwithbasin.obj");
  Mesh *table = loadFile("../obj files/p_tablecuisine.obj");
  Mesh *table_chair = loadFile("../obj files/p_chaise.obj");

  calculateNormalPerFace(bed);
  calculateNormalPerVertex(bed);
  calculateNormalPerVertex(bookshelf);
  calculateNormalPerVertex(bookshelf);
  calculateNormalPerFace(ceiling_light);
  calculateNormalPerVertex(ceiling_light);
  calculateNormalPerFace(desk);
  calculateNormalPerVertex(desk);
  calculateNormalPerFace(desk_light);
  calculateNormalPerVertex(desk_light);
  calculateNormalPerFace(floor_lamp);
  calculateNormalPerVertex(floor_lamp);
  calculateNormalPerFace(fluorescent_light);
  calculateNormalPerVertex(fluorescent_light);
  calculateNormalPerFace(hanging_light);
  calculateNormalPerVertex(hanging_light);
  calculateNormalPerFace(sink);
  calculateNormalPerVertex(sink);
  calculateNormalPerFace(table);
  calculateNormalPerVertex(table);
  calculateNormalPerFace(table_chair);
  calculateNormalPerVertex(table_chair);

  bed_list = meshToDisplayList(bed, 12);
  bookshelf_list = meshToDisplayList(bookshelf, 12);
  ceiling_light_list = meshToDisplayList(ceiling_light, 4);
  desk_list = meshToDisplayList(desk, 5);
  desk_light_list = meshToDisplayList(desk_light, 6);
  floor_lamp_list = meshToDisplayList(floor_lamp, 6);
  fluorescent_light_list = meshToDisplayList(fluorescent_light, 6);
  hanging_light_list = meshToDisplayList(hanging_light, 10);
  sink_list = meshToDisplayList(sink, 4);
  table_list = meshToDisplayList(table, 12);
  table_chair_list = meshToDisplayList(table_chair, 12);

  initWorld();

  setLighting(0, 0);
}

void initHallway()
{
  Mesh *floor_mesh = createPlane(200, 1000, 50);
  Mesh *ceiling_mesh = createPlane(200, 1000, 50);
  Mesh *small_wall_mesh = createPlane(100, 100, 50);
  Mesh *medium_wall_mesh = createPlane(200, 100, 50);
  Mesh *long_wall_mesh = createPlane(100, 600, 50);

  calculateNormalPerFace(floor_mesh);
  calculateNormalPerFace(ceiling_mesh);
  calculateNormalPerFace(small_wall_mesh);
  calculateNormalPerFace(medium_wall_mesh);
  calculateNormalPerFace(long_wall_mesh);

  calculateNormalPerVertex(floor_mesh);
  calculateNormalPerVertex(ceiling_mesh);
  calculateNormalPerVertex(small_wall_mesh);
  calculateNormalPerVertex(medium_wall_mesh);
  calculateNormalPerVertex(long_wall_mesh);

  GLuint floor = meshToDisplayList(floor_mesh, 2);
  GLuint ceiling = meshToDisplayList(ceiling_mesh, 3);
  GLuint small_wall = meshToDisplayList(small_wall_mesh, 3);
  GLuint medium_wall = meshToDisplayList(medium_wall_mesh, 3);
  GLuint long_wall = meshToDisplayList(long_wall_mesh, 3);

  hallway_list = glGenLists(1);
  glNewList(hallway_list, GL_COMPILE);

  // FLOOR
  glPushMatrix();
  glTranslatef(-100.0f, 0.0f, -500.0f);
  glCallList(floor);
  glPopMatrix();

  // ceiling
  glPushMatrix();
  glTranslatef(100.0f, 100.0f, -500.0f);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  glCallList(ceiling);
  glPopMatrix();

  // left wall
  glPushMatrix();
  glTranslatef(-100.0f, 100.0f, -500.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glCallList(small_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-100.0f, 100.0f, -300.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glCallList(long_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-100.0f, 100.0f, 400.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glCallList(small_wall);
  glPopMatrix();

  // right wall
  glPushMatrix();
  glTranslatef(100.0f, 0.0f, -500.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(small_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(100.0f, 0.0f, -300.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(long_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(100.0f, 0.0f, 400.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(small_wall);
  glPopMatrix();

  // back wall
  glPushMatrix();
  glTranslatef(-100.0f, 100.0f, -500.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(medium_wall);
  glPopMatrix();

  // front wall
  glPushMatrix();
  glTranslatef(-100.0f, 000.0f, 500.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(medium_wall);
  glPopMatrix();

  glEndList();

  // hallway lighting
  GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light_diffuse[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light_specular[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  //glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00001f);
}

void initRoom1()
{
  Mesh *floor_mesh = createPlane(400, 300, 50);
  Mesh *ceiling_mesh = createPlane(400, 300, 50);
  Mesh *door_wall_mesh = createPlane(50, 100, 50);
  Mesh *north_south_wall_mesh = createPlane(350, 100, 50);
  Mesh *west_wall_mesh = createPlane(100, 300, 50);
  Mesh *east_wall_mesh = createPlane(100, 100, 50);

  calculateNormalPerFace(floor_mesh);
  calculateNormalPerFace(ceiling_mesh);
  calculateNormalPerFace(door_wall_mesh);
  calculateNormalPerFace(north_south_wall_mesh);
  calculateNormalPerFace(west_wall_mesh);
  calculateNormalPerFace(east_wall_mesh);

  calculateNormalPerVertex(floor_mesh);
  calculateNormalPerVertex(ceiling_mesh);
  calculateNormalPerVertex(door_wall_mesh);
  calculateNormalPerVertex(north_south_wall_mesh);
  calculateNormalPerVertex(west_wall_mesh);
  calculateNormalPerVertex(east_wall_mesh);

  GLuint floor = meshToDisplayList(floor_mesh, 14);
  GLuint ceiling = meshToDisplayList(ceiling_mesh, 3);
  GLuint door_wall = meshToDisplayList(door_wall_mesh, 3);
  GLuint north_south_wall = meshToDisplayList(north_south_wall_mesh, 3);
  GLuint west_wall = meshToDisplayList(west_wall_mesh, 3);
  GLuint east_wall = meshToDisplayList(east_wall_mesh, 3);

  room1_list = glGenLists(1);
  glNewList(room1_list, GL_COMPILE);

  // floor
  glPushMatrix();
  glTranslatef(-500.0f, 0.0f, -500.0f);
  glCallList(floor);
  glPopMatrix();

  // ceiling
  glPushMatrix();
  glTranslatef(-100.0f, 100.0f, -500.0f);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  glCallList(ceiling);
  glPopMatrix();

  // doorway
  glPushMatrix();
  glTranslatef(-150.0f, 100.0f, -400.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-150.0f, 0.0f, -300.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  // east wall
  glPushMatrix();
  glTranslatef(-150.0f, 0.0f, -500.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-150.0f, 0.0f, -300.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  // north wall
  glPushMatrix();
  glTranslatef(-500.0f, 0.0f, -500.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // south wall
  glPushMatrix();
  glTranslatef(-500.0f, 100.0f, -200.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // west wall
  glPushMatrix();
  glTranslatef(-500.0f, 100.0f, -500.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glCallList(west_wall);
  glPopMatrix();

  glEndList();
}

void initRoom2()
{
  Mesh *floor_mesh = createPlane(400, 300, 50);
  Mesh *ceiling_mesh = createPlane(400, 300, 50);
  Mesh *door_wall_mesh = createPlane(50, 100, 50);
  Mesh *north_south_wall_mesh = createPlane(350, 100, 50);
  Mesh *west_wall_mesh = createPlane(100, 300, 50);
  Mesh *east_wall_mesh = createPlane(100, 100, 50);

  calculateNormalPerFace(floor_mesh);
  calculateNormalPerFace(ceiling_mesh);
  calculateNormalPerFace(door_wall_mesh);
  calculateNormalPerFace(north_south_wall_mesh);
  calculateNormalPerFace(west_wall_mesh);
  calculateNormalPerFace(east_wall_mesh);

  calculateNormalPerVertex(floor_mesh);
  calculateNormalPerVertex(ceiling_mesh);
  calculateNormalPerVertex(door_wall_mesh);
  calculateNormalPerVertex(north_south_wall_mesh);
  calculateNormalPerVertex(west_wall_mesh);
  calculateNormalPerVertex(east_wall_mesh);

  GLuint floor = meshToDisplayList(floor_mesh, 13);
  GLuint ceiling = meshToDisplayList(ceiling_mesh, 3);
  GLuint door_wall = meshToDisplayList(door_wall_mesh, 3);
  GLuint north_south_wall = meshToDisplayList(north_south_wall_mesh, 3);
  GLuint west_wall = meshToDisplayList(west_wall_mesh, 3);
  GLuint east_wall = meshToDisplayList(east_wall_mesh, 3);

  room2_list = glGenLists(1);
  glNewList(room2_list, GL_COMPILE);

  // floor
  glPushMatrix();
  glTranslatef(-500.0f, 0.0f, 200.0f);
  glCallList(floor);
  glPopMatrix();

  // ceiling
  glPushMatrix();
  glTranslatef(-100.0f, 100.0f, 200.0f);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  glCallList(ceiling);
  glPopMatrix();

  // doorway
  glPushMatrix();
  glTranslatef(-150.0f, 100.0f, 300.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-150.0f, 0.0f, 400.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  // east wall
  glPushMatrix();
  glTranslatef(-150.0f, 0.0f, 200.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-150.0f, 0.0f, 400.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  // north wall
  glPushMatrix();
  glTranslatef(-500.0f, 0.0f, 200.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // south wall
  glPushMatrix();
  glTranslatef(-500.0f, 100.0f, 500.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // west wall
  glPushMatrix();
  glTranslatef(-500.0f, 100.0f, 200.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glCallList(west_wall);
  glPopMatrix();

  glEndList();
}

void initRoom3()
{
  Mesh *floor_mesh = createPlane(400, 300, 50);
  Mesh *ceiling_mesh = createPlane(400, 300, 50);
  Mesh *door_wall_mesh = createPlane(50, 100, 50);
  Mesh *north_south_wall_mesh = createPlane(350, 100, 50);
  Mesh *west_wall_mesh = createPlane(100, 300, 50);
  Mesh *east_wall_mesh = createPlane(100, 100, 50);

  calculateNormalPerFace(floor_mesh);
  calculateNormalPerFace(ceiling_mesh);
  calculateNormalPerFace(door_wall_mesh);
  calculateNormalPerFace(north_south_wall_mesh);
  calculateNormalPerFace(west_wall_mesh);
  calculateNormalPerFace(east_wall_mesh);

  calculateNormalPerVertex(floor_mesh);
  calculateNormalPerVertex(ceiling_mesh);
  calculateNormalPerVertex(door_wall_mesh);
  calculateNormalPerVertex(north_south_wall_mesh);
  calculateNormalPerVertex(west_wall_mesh);
  calculateNormalPerVertex(east_wall_mesh);

  GLuint floor = meshToDisplayList(floor_mesh, 13);
  GLuint ceiling = meshToDisplayList(ceiling_mesh, 3);
  GLuint door_wall = meshToDisplayList(door_wall_mesh, 3);
  GLuint north_south_wall = meshToDisplayList(north_south_wall_mesh, 3);
  GLuint west_wall = meshToDisplayList(west_wall_mesh, 3);
  GLuint east_wall = meshToDisplayList(east_wall_mesh, 3);

  room3_list = glGenLists(1);
  glNewList(room3_list, GL_COMPILE);

  // floor
  glPushMatrix();
  glTranslatef(100.0f, 0.0f, -500.0f);
  glCallList(floor);
  glPopMatrix();

  // ceiling
  glPushMatrix();
  glTranslatef(500.0f, 100.0f, -500.0f);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  glCallList(ceiling);
  glPopMatrix();

  // doorway
  glPushMatrix();
  glTranslatef(100.0f, 100.0f, -400.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(100.0f, 0.0f, -300.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  // west wall
  glPushMatrix();
  glTranslatef(150.0f, 0.0f, -500.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(150.0f, 0.0f, -300.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  // north wall
  glPushMatrix();
  glTranslatef(150.0f, 0.0f, -500.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // south wall
  glPushMatrix();
  glTranslatef(150.0f, 100.0f, -200.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // east wall
  glPushMatrix();
  glTranslatef(500.0f, 100.0f, -500.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glCallList(west_wall);
  glPopMatrix();

  glEndList();
}

void initRoom4()
{
  Mesh *floor_mesh = createPlane(400, 300, 50);
  Mesh *ceiling_mesh = createPlane(400, 300, 50);
  Mesh *door_wall_mesh = createPlane(50, 100, 50);
  Mesh *north_south_wall_mesh = createPlane(350, 100, 50);
  Mesh *west_wall_mesh = createPlane(100, 300, 50);
  Mesh *east_wall_mesh = createPlane(100, 100, 50);

  calculateNormalPerFace(floor_mesh);
  calculateNormalPerFace(ceiling_mesh);
  calculateNormalPerFace(door_wall_mesh);
  calculateNormalPerFace(north_south_wall_mesh);
  calculateNormalPerFace(west_wall_mesh);
  calculateNormalPerFace(east_wall_mesh);

  calculateNormalPerVertex(floor_mesh);
  calculateNormalPerVertex(ceiling_mesh);
  calculateNormalPerVertex(door_wall_mesh);
  calculateNormalPerVertex(north_south_wall_mesh);
  calculateNormalPerVertex(west_wall_mesh);
  calculateNormalPerVertex(east_wall_mesh);

  GLuint floor = meshToDisplayList(floor_mesh, 14);
  GLuint ceiling = meshToDisplayList(ceiling_mesh, 14);
  GLuint door_wall = meshToDisplayList(door_wall_mesh, 14);
  GLuint north_south_wall = meshToDisplayList(north_south_wall_mesh, 14);
  GLuint west_wall = meshToDisplayList(west_wall_mesh, 14);
  GLuint east_wall = meshToDisplayList(east_wall_mesh, 14);

  room4_list = glGenLists(1);
  glNewList(room4_list, GL_COMPILE);

  // floor
  glPushMatrix();
  glTranslatef(100.0f, 0.0f, 200.0f);
  glCallList(floor);
  glPopMatrix();

  // ceiling
  glPushMatrix();
  glTranslatef(500.0f, 100.0f, 200.0f);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  glCallList(ceiling);
  glPopMatrix();

  // doorway
  glPushMatrix();
  glTranslatef(100.0f, 100.0f, 300.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(100.0f, 0.0f, 400.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(door_wall);
  glPopMatrix();

  // west wall
  glPushMatrix();
  glTranslatef(150.0f, 0.0f, 200.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(150.0f, 0.0f, 400.0f);
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glCallList(east_wall);
  glPopMatrix();

  // north wall
  glPushMatrix();
  glTranslatef(150.0f, 0.0f, 200.0f);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // south wall
  glPushMatrix();
  glTranslatef(150.0f, 100.0f, 500.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glCallList(north_south_wall);
  glPopMatrix();

  // east wall
  glPushMatrix();
  glTranslatef(500.0f, 100.0f, 200.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glCallList(west_wall);
  glPopMatrix();

  glEndList();
}

void initWorld()
{
  initHallway();
  initRoom1(); // northwest room
  initRoom2(); // southwest room
  initRoom3(); // northeast room
  initRoom4(); // southeast room
}

void setLighting(int x, int y)
{
  //int light_setting = nav_map[x][y];
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);
  glEnable(GL_LIGHT4);
  glEnable(GL_LIGHT5);
  glEnable(GL_LIGHT6);
}

// Display the scene
void myDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // main view
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, screen_width, screen_height);
  gluPerspective(45, screen_ratio, 1, 3000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(player_x, player_y, player_z,
    player_x + sin(player_angle), player_y, player_z + cos(player_angle),
    0.0f, 1.0f, 0.0f);

  glPushMatrix();
  glCallList(hallway_list);
  glCallList(room1_list);
  glCallList(room2_list);
  glCallList(room3_list);
  glCallList(room4_list);
  glPopMatrix();

  // southwest room objects
  glPushMatrix();
  glTranslatef(-510.0f, 0.0f, 200.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(desk_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-260.0f, 27.0f, 230.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(desk_light_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-450.0f, 20.0f, 450.0f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glScalef(50.0f, 50.0f, 50.0f);
  glCallList(bed_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-450.0f, 0.0f, 350.0f);
  glScalef(0.5f, 0.5f, 0.5f);
  glCallList(floor_lamp_list);
  GLfloat light_ambient3[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_diffuse3[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular3[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position3[] = { 0.0, 270.0, 0.0, 1.0 };
  GLfloat light_direction3[] = { 0.0f, -1.0f, 0.0f };
  glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
  glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
  glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
  glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
  glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_direction3);
  glLighti(GL_LIGHT3, GL_SPOT_EXPONENT, 5);
  glLighti(GL_LIGHT3, GL_SPOT_CUTOFF, 90);
  glPopMatrix();

  GLfloat light_ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
  GLfloat light_diffuse1[] = { 0.5, 0.5, 0.5, 1.0 };
  GLfloat light_specular1[] = { 0.5, 0.5, 0.5, 1.0 };
  GLfloat light_position1[] = { -260.0, 60.0, 230.0, 1.0 };
  GLfloat light_direction1[] = { 0.0f, -1.0f, 0.0f };
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction1);
  glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, 5);
  glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, 90);

  // northwest room objects
  glPushMatrix();
  glTranslatef(-330.0f, 0.0f, -350.0f);
  glScalef(0.5f, 0.5f, 0.5f);
  glCallList(table_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-330.0f, 15.0f, -350.0f);
  glTranslatef(0.0f, 0.0f, -40.0f);
  glScalef(15.0f, 15.0f, 15.0f);
  glCallList(table_chair_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-330.0f, 15.0f, -350.0f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, -60.0f);
  glScalef(15.0f, 15.0f, 15.0f);
  glCallList(table_chair_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-330.0f, 15.0f, -350.0f);
  glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, -40.0f);
  glScalef(15.0f, 15.0f, 15.0f);
  glCallList(table_chair_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-330.0f, 15.0f, -350.0f);
  glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, -60.0f);
  glScalef(15.0f, 15.0f, 15.0f);
  glCallList(table_chair_list);
  glPopMatrix();

  float display_angle = getHangingLightAngle();

  glPushMatrix();
  glTranslatef(-330.0f, 100.0f, -350.0f);
  glRotatef(display_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -50.0f, 0.0f);
  glScalef(7.0f, 7.0f, 7.0f);
  glCallList(hanging_light_list);
  glTranslatef(0.0f, 45.0f, 0.0f);
  GLfloat light_ambient2[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light_diffuse2[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light_specular2[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat light_position2[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_direction2[] = { 0.0f, -1.0f, 0.0f };
  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
  glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction2);
  glLighti(GL_LIGHT2, GL_SPOT_EXPONENT, 100);
  glLighti(GL_LIGHT2, GL_SPOT_CUTOFF, 45);
  glPopMatrix();

  // southeast room objects
  glPushMatrix();
  glTranslatef(450.0f, 0.0f, 300.0f);
  glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
  glScalef(0.5f, 0.5f, 0.5f);
  glCallList(sink_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(450.0f, 0.0f, 400.0f);
  glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
  glScalef(0.5f, 0.5f, 0.5f);
  glCallList(sink_list);
  glPopMatrix();

  GLfloat mat_emission1[] = { 0.5, 0.0, 0.0, 1.0 };
  glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission1);

  glPushMatrix();
  glTranslatef(150.0f, 50.0f, 250.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glScalef(20.0f, 20.0f, 20.0f);
  glCallList(fluorescent_light_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(150.0f, 50.0f, 450.0f);
  glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
  glScalef(20.0f, 20.0f, 20.0f);
  glCallList(fluorescent_light_list);
  glPopMatrix();

  GLfloat mat_emission2[] = { 0.0, 0.0, 0.0, 1.0 };
  glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission2);

  GLfloat light_ambient4[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse4[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_specular4[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_position4[] = { 140.0f, 50.0f, 250.0f, 1.0 };
  GLfloat light_direction4[] = { 1.0f, 0.0f, 0.0f };
  glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient4);
  glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
  glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular4);
  glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
  glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_direction4);
  glLighti(GL_LIGHT4, GL_SPOT_EXPONENT, 10);
  glLighti(GL_LIGHT4, GL_SPOT_CUTOFF, 90);

  GLfloat light_ambient5[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse5[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_specular5[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_position5[] = { 140.0f, 50.0f, 450.0f, 1.0 };
  GLfloat light_direction5[] = { 1.0f, 0.0f, 0.0f };
  glLightfv(GL_LIGHT5, GL_AMBIENT, light_ambient5);
  glLightfv(GL_LIGHT5, GL_DIFFUSE, light_diffuse5);
  glLightfv(GL_LIGHT5, GL_SPECULAR, light_specular5);
  glLightfv(GL_LIGHT5, GL_POSITION, light_position5);
  glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, light_direction5);
  glLighti(GL_LIGHT5, GL_SPOT_EXPONENT, 10);
  glLighti(GL_LIGHT5, GL_SPOT_CUTOFF, 90);

  // northeast room
  glPushMatrix();
  glTranslatef(350.0f, 90.0f, -350.0f);
  glScalef(0.2f, 0.2f, 0.2f);
  glCallList(ceiling_light_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(225.0f, 50.0f, -380.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(300.0f, 50.0f, -380.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(375.0f, 50.0f, -380.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(450.0f, 50.0f, -380.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(225.0f, 50.0f, -115.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(300.0f, 50.0f, -115.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(375.0f, 50.0f, -115.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(450.0f, 50.0f, -115.0f);
  glScalef(0.4f, 0.4f, 0.4f);
  glCallList(bookshelf_list);
  glPopMatrix();

  GLfloat light_ambient6[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_diffuse6[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular6[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position6[] = { 350.0f, 500.0f, -350.0f, 1.0 };
  GLfloat light_direction6[] = { 0.0f, -1.0f, 0.0f };
  glLightfv(GL_LIGHT6, GL_AMBIENT, light_ambient6);
  glLightfv(GL_LIGHT6, GL_DIFFUSE, light_diffuse6);
  glLightfv(GL_LIGHT6, GL_SPECULAR, light_specular6);
  glLightfv(GL_LIGHT6, GL_POSITION, light_position6);
  glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, light_direction6);
  glLighti(GL_LIGHT6, GL_SPOT_EXPONENT, 15);
  glLighti(GL_LIGHT6, GL_SPOT_CUTOFF, 90);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  // overhead map
  int map_size;

  if (screen_ratio < 1)
  {
    map_size = screen_width / 5;
  }
  else
  {
    map_size = screen_height / 5;
  }
  int map_offset = map_size / 2;
  //glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glViewport(screen_width - map_offset - map_size, map_offset, map_size, map_size);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-500.0f, 500.0f, -500.0f, 500.0f, 1, 1000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0f, 50.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f);

  // draw black background on map
  glPushMatrix();
  glBegin(GL_TRIANGLES);
  glColor3f(0.0f, 0.0f, 0.0f);
  glVertex3f(-500.0f, -500.0f, -500.0f);
  glVertex3f(-500.0f, -500.0f, 500.0f);
  glVertex3f(500.0f, -500.0f, -500.0f);
  glVertex3f(500.0f, -500.0f, -500.0f);
  glVertex3f(-500.0f, -500.0f, 500.0f);
  glVertex3f(500.0f, -500.0f, 500.0f);
  glEnd();
  glPopMatrix();
  glColor3f(1.0f, 1.0f, 1.0f);

  // draw world
  glCallList(hallway_list);
  glCallList(room1_list);
  glCallList(room2_list);
  glCallList(room3_list);
  glCallList(room4_list);

  // draw character on map
  glPushMatrix();
  glTranslatef(player_x, 0.0f, player_z);
  glRotatef((player_angle / PI * 180) + 180, 0.0f, 1.0f, 0.0f);

  glBegin(GL_TRIANGLES);
  glVertex3f(0.0f, 10.0f, -50.0f);
  glVertex3f(-20.0f, 10.0f, 50.0f);
  glVertex3f(20.0f, 10.0f, 50.0f);
  glEnd();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glutSwapBuffers();
}

// Handler for animating the scene
void myIdleFunc()
{
  int current_time = glutGet(GLUT_ELAPSED_TIME);
  int elapsed_time = current_time - last_time_stamp;
  last_time_stamp = current_time;
  float seconds = (float)elapsed_time / 1000.0f;

  float angle_change = HANGING_LIGHT_RANGE * (seconds / HANGING_LIGHT_PERIOD);
  hanging_light_angle += angle_change;

  while (hanging_light_angle > HANGING_LIGHT_RANGE * 4)
  {
    hanging_light_angle -= HANGING_LIGHT_RANGE * 4;
  }

  glutPostRedisplay();
}

// Handler for keyboard buttons
void mySpecialFunc(int key, int x, int y)
{
  float new_x = player_x;
  float new_z = player_z;

  switch (key)
  {
  case GLUT_KEY_UP:
    new_x += sin(player_angle) * player_move_speed;
    new_z += cos(player_angle) * player_move_speed;
    break;
  case GLUT_KEY_LEFT:
    player_angle += player_rotate_speed;
    break;
  case GLUT_KEY_DOWN:
    new_x += (-sin(player_angle)) * player_move_speed;
    new_z += (-cos(player_angle)) * player_move_speed;
    break;
  case GLUT_KEY_RIGHT:
    player_angle -= player_rotate_speed;
    break;
  }

  while (player_angle > 2.0f * PI)
  {
    player_angle -= 2.0f * PI;
  }

  while (player_angle < 0.0f)
  {
    player_angle += 2.0f * PI;
  }

  int loc_x = (new_x / GRID_SIZE) + 25;
  int loc_z = (new_z / GRID_SIZE) + 25;

  if (loc_x >= 0 && loc_x < 50 && loc_z >= 0 && loc_z < 50 && nav_map[loc_z][loc_x] == 0)
  {
    player_x = new_x;
    player_z = new_z;
  }

  glutPostRedisplay();
}

// Handler for window resizing
void myReshapeFunc(int x, int y)
{
  screen_width = x;
  screen_height = y;

  if (screen_height == 0)
  {
    screen_height = 1;
  }

  screen_ratio = (float)x / (float)y;
  glutPostRedisplay();
}

// str to int
int StrToInt(const string &str) {
  int i;
  if (sscanf_s(str.c_str(), "%i", &i) == 1) return i;
  else return 0;
}

// split string
vector<string> split_string(const string& str, const string& split_str) {
  vector<string> stlv_string;
  string part_string("");
  string::size_type i;
  i = 0;
  while (i < str.size()) {
    if (split_str.find(str[i]) != string::npos) {
      stlv_string.push_back(part_string);
      part_string = "";
      while (split_str.find(str[i]) != string::npos) {
        ++i;
      }
    }
    else {
      part_string += str[i];
      ++i;
    }
  }
  if (!part_string.empty())
    stlv_string.push_back(part_string);
  return stlv_string;
}

// load file
Mesh* loadFile(const char* file) {
  Mesh *m = new Mesh;
  m->dot_vertex.clear();
  m->face_index_vertex.clear();
  ifstream infile(file);
  if (infile.fail()) {
    cout << "Error opening file " << file;
    return NULL;
  }
  char current_line[1024];
  while (!infile.eof()) {
    infile.getline(current_line, 1024);
    switch (current_line[0]) {
    case 'v':
      float x, y, z;
      switch (current_line[1]) {
      case 'n':
        sscanf_s(current_line, "vn %f %f %f", &x, &y, &z);
        m->dot_normalPerFace.push_back(Vec3f(x, y, z));
        m->dot_normalPerVertex.push_back(Vec3f(x, y, z));
        break;
      case 't':
        sscanf_s(current_line, "vt %f %f", &x, &y);
        m->dot_texture.push_back(Vec2f(x, y));
        break;
      default:
        sscanf_s(current_line, "v %f %f %f", &x, &y, &z);
        m->dot_vertex.push_back(Vec3f(x, y, z));
        break;
      }
      break;
    case 'f': {
      vector<string> faces = split_string(current_line, " \t.\r\n");
      int vnt[3][3] = { { -1, -1, -1 },{ -1, -1, -1 },{ -1, -1, -1 } };
      string::size_type begin, end;
      for (int i = 0; i < 2; i++) {
        begin = 0;
        int j = 0;
        do {
          end = faces[i + 1].find_first_of("/", begin);
          if (begin < end) {
            vnt[i][j] = StrToInt(faces[i + 1].substr(begin, end - begin)) - 1;
          }
          else {
            vnt[i][j] = -1;
          }
          begin = end + 1;
          j++;
        } while (end != string::npos);
      }
      for (unsigned int i = 3; i < faces.size(); i++) {
        begin = 0;
        int j = 0;
        do {
          end = faces[i].find_first_of("/", begin);
          if (begin < end) {
            vnt[2][j] = StrToInt(faces[i].substr(begin, end - begin)) - 1;
          }
          else {
            vnt[2][j] = -1;
          }
          begin = end + 1;
          j++;
        } while (end != string::npos);
        for (int j = 0; j < 3; j++) {
          m->face_index_vertex.push_back(vnt[j][0]);
          if (vnt[j][1] != -1) m->face_index_texture.push_back(vnt[j][1]);
          if (vnt[j][2] != -1) m->face_index_normalPerFace.push_back(vnt[j][2]);
          if (vnt[j][2] != -1) m->face_index_normalPerVertex.push_back(vnt[j][2]);
        }
        memcpy(&vnt[1], &vnt[2], sizeof(int) * 3);
      }
    }
              break;
    default:
      break;
    }
  }

  return m;
}

// normal per face
void calculateNormalPerFace(Mesh* m) {
  Vec3<float> v1, v2, v3, v4, v5;
  for (int i = 0; i < m->face_index_vertex.size(); i += 3) {
    v1 = m->dot_vertex[m->face_index_vertex[i]];
    v2 = m->dot_vertex[m->face_index_vertex[i + 1]];
    v3 = m->dot_vertex[m->face_index_vertex[i + 2]];
    v4 = (v2 - v1);
    v5 = (v3 - v1);
    v4 = v4.cross(v5);
    v4.normalize();
    m->dot_normalPerFace.push_back(v4);
    int pos = m->dot_normalPerFace.size() - 1;
    // same normal for all vertex in this face
    m->face_index_normalPerFace.push_back(pos);
    m->face_index_normalPerFace.push_back(pos);
    m->face_index_normalPerFace.push_back(pos);
  }
}

// calculate normal per vertex
void calculateNormalPerVertex(Mesh* m) {
  m->dot_normalPerVertex.clear();
  m->face_index_normalPerVertex.clear();
  Vec3<float> suma; suma.x = 0; suma.y = 0; suma.z = 0;
  //initialize
  for (unsigned int val = 0; val < m->dot_vertex.size(); val++) {
    m->dot_normalPerVertex.push_back(suma);
  }
  // calculate sum for vertex
  for (long pos = 0; pos < m->face_index_vertex.size(); pos++) {
    m->dot_normalPerVertex[m->face_index_vertex[pos]] +=
      m->dot_normalPerFace[m->face_index_normalPerFace[pos]];
  }
  // normalize for vertex 
  for (unsigned int val = 0; val < m->dot_normalPerVertex.size(); val++) {
    m->dot_normalPerVertex[val] = m->dot_normalPerVertex[val].normalize();
  }
  //normalVertexIndex is the same that vertexIndex
  for (unsigned int pos = 0; pos < m->face_index_vertex.size(); pos++) {
    m->face_index_normalPerVertex.push_back(m->face_index_vertex[pos]);
  }
}

// creating a triangulated plane
Mesh* createPlane(int arena_width, int arena_depth, int arena_cell) {
  Mesh *me = new Mesh;
  int n = arena_width / arena_cell + 1;
  int m = arena_depth / arena_cell + 1;

  // vertices
  for (int j = 0; j<m; j++) {
    for (int i = 0; i < n; i++) {
      me->dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, 0.0, j*arena_cell));
    }
  }
  //texture
  me->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
  me->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
  me->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
  me->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
  // faces
  for (int i = 0; i<(n*m) - n; i++) {
    if ((i + 1) % n == 0) continue;
    me->face_index_vertex.push_back(i); me->face_index_vertex.push_back(i + n);
    me->face_index_vertex.push_back(i + 1);
    me->face_index_vertex.push_back(i + 1); me->face_index_vertex.push_back(i + n);
    me->face_index_vertex.push_back(i + n + 1);
    // index for texture
    for (int t = 0; t<6; t++) {
      me->face_index_texture.push_back(3);
      me->face_index_texture.push_back(1);
      me->face_index_texture.push_back(2);
      me->face_index_texture.push_back(2);
      me->face_index_texture.push_back(1);
      me->face_index_texture.push_back(0);
    }
  }
  return me;
}

// draw
GLuint meshToDisplayList(Mesh* m, int id) {
  GLuint listID = glGenLists(id);
  glNewList(listID, GL_COMPILE);
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texture_array[id - 1]);
  glBegin(GL_TRIANGLES);

  for (unsigned int i = 0; i < m->face_index_vertex.size(); i++) {
    // PER VERTEX NORMALS
    if ((!m->dot_normalPerVertex.empty() && !m->face_index_normalPerVertex.empty())) {
      glNormal3fv(&m->dot_normalPerVertex[m->face_index_normalPerVertex[i]].x);
    }
    if (!m->dot_texture.empty() && !m->face_index_texture.empty() && i < m->face_index_texture.size()) {
      glTexCoord2fv(&m->dot_texture[m->face_index_texture[i]].x);
    }
    // color
    Vec3f offset = (m->dot_vertex[m->face_index_vertex[i]]);
    //
    glVertex3fv(&m->dot_vertex[m->face_index_vertex[i]].x);
  }

  glEnd();
  glDisable(GL_TEXTURE_2D);
  glEndList();
  return listID;
}

// Load a DIB/BMP file from disk.
GLubyte *LoadDIBitmap(const char *filename, BITMAPINFO **info) {
  FILE *fp;      // open file pointer
  GLubyte * bits; // bitmap pixel bits
  int bitsize;   // Size of bitmap
  int infosize;  // Size of header information
  BITMAPFILEHEADER header; // File header
                           // try opening the file; use "rb" mode to read this *binary* file.
  if ((fp = fopen(filename, "rb")) == NULL)
    return (NULL);
  // read the file header and any following bitmap information.
  if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
    // Couldn't read the file header - return NULL.
    fclose(fp);
    return (NULL);
  }
  // Check for BM reversed.
  if (header.bfType != 'MB') {
    // not a bitmap file - return NULL.
    fclose(fp);
    return (NULL);
  }
  infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
  if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
    // couldn't allocate memory for bitmap info - return NULL.
    fclose(fp);
    return (NULL);
  }
  if (fread(*info, 1, infosize, fp) < infosize) {
    // Couldn't read the bitmap header - return NULL.
    free(*info);
    fclose(fp);
    return (NULL);
  }
  // Now that we have all the header info read in, allocate memory for the bitmap and read *it* in.
  if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
    bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8 * abs((*info)->bmiHeader.biHeight);
  if ((bits = (GLubyte *)malloc(bitsize)) == NULL) {
    // Couldn't allocate memory - return NULL!
    free(*info);
    fclose(fp);
    return (NULL);
  }
  if (fread(bits, 1, bitsize, fp) < bitsize) {
    // couldn't read bitmap - free memory and return NULL!
    free(*info);
    free(bits);
    fclose(fp);
    return (NULL);
  }
  // OK, everything went fine - return the allocated bitmap.
  fclose(fp);
  return (bits);
}

// bmpTexture
void bmpTexture(UINT textureArray[], LPSTR file, int n) {
  BITMAPINFO *bitmapInfo; // Bitmap information
  GLubyte    *bitmapBits; // Bitmap data
  if (!file) {
    cout << "texture file not found!" << endl;
    return;
  }
  bitmapBits = LoadDIBitmap(file, &bitmapInfo);
  glGenTextures(1, &textureArray[n]);
  glBindTexture(GL_TEXTURE_2D, textureArray[n]);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
                                         // glTexImage2D Whith size and minification
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmapBits);
}

// Create from algorithm
void codedTexture(UINT textureArray[], int n, int type, float amp, float frequency, float phase) {
  const int TexHeight = 128;
  const int TexWidth = 128;
  // create texture in memory
  GLubyte textureImage[TexHeight][TexWidth][4];
  for (int i = 0; i < TexHeight; i++)
    for (int j = 0; j < TexWidth; j++) {

      double noise;
      if (type == MULTISCALE_BLUE)
      {
        noise = amp * iNoise.perlinMultiscale(frequency * (float)i + phase, frequency * (float)j + phase);
        textureImage[i][j][0] = int(noise * (float)255);
        textureImage[i][j][1] = 0;
        textureImage[i][j][2] = 0;
        textureImage[i][j][3] = 1;
      }
      else if (type == MULTISCALE_WHITE)
      {
        noise = amp * iNoise.perlinMultiscale(frequency * (float)i + phase, frequency * (float)j + phase);
        textureImage[i][j][0] = int(noise * (float)255);
        textureImage[i][j][1] = int(noise * (float)255);
        textureImage[i][j][2] = int(noise * (float)255);
        textureImage[i][j][3] = 1;
      }
      else if (type == MULTISCALE_RED)
      {
        noise = amp * iNoise.perlinMultiscale(frequency * (float)i + phase, frequency * (float)j + phase);
        textureImage[i][j][0] = 0;
        textureImage[i][j][1] = 0;
        textureImage[i][j][2] = int(noise * (float)255);
        textureImage[i][j][3] = 1;
      }
      else if (type == MULTISCALE_GREEN)
      {
        noise = amp * iNoise.perlinMultiscale(frequency * (float)i + phase, frequency * (float)j + phase);
        textureImage[i][j][0] = 0;
        textureImage[i][j][1] = int(noise * (float)255);
        textureImage[i][j][2] = 0;
        textureImage[i][j][3] = 1;
      }
      else if (type == MARBLE)
      {
        noise = amp * iNoise.perlinMarble(frequency * (float)i + phase, frequency * (float)j + phase);
        textureImage[i][j][0] = int(noise * (float)255);
        textureImage[i][j][1] = int(noise * (float)255);
        textureImage[i][j][2] = int(noise * (float)255);
        textureImage[i][j][3] = 1;
      }
    }
  // setup texture
  glGenTextures(1, &textureArray[n]);
  glBindTexture(GL_TEXTURE_2D, textureArray[n]);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
                                         // glTexImage2D Whith size and minification
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TexWidth, TexHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureImage); // BGRA to include alpha

}

float getHangingLightAngle()
{
  float angle = 0.0f;
  if (hanging_light_angle < HANGING_LIGHT_RANGE)
  {
    angle = hanging_light_angle;
  }
  else if (hanging_light_angle < HANGING_LIGHT_RANGE * 2)
  {
    angle = HANGING_LIGHT_RANGE * 2 - hanging_light_angle;
  }
  else if (hanging_light_angle < HANGING_LIGHT_RANGE * 3)
  {
    angle = -(hanging_light_angle - HANGING_LIGHT_RANGE * 2);
  }
  else if (hanging_light_angle < HANGING_LIGHT_RANGE * 4)
  {
    angle = -(HANGING_LIGHT_RANGE * 4 - hanging_light_angle);
  }

  return angle;
}
