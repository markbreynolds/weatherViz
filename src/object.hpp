#ifndef _OBJECT
#define _OBJECT

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include <glm/glm.hpp>

using namespace std;

bool loadOBJ(istream &stream, vector<glm::vec3> &vertices, vector<glm::vec3> &normals);
bool loadOBJString(string String, vector<glm::vec3> &vertices, vector<glm::vec3> &normals);
bool loadOBJFile(string path, vector<glm::vec3> &vertices, vector<glm::vec3> &normals);

#endif
