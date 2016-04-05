#ifndef _IMAGE
#define _IMAGE

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <lodepng.h>

using namespace std;

void pngInfo(string imageFile);
GLuint pngTexture(string imageFile);

#endif
