#include <exception>

#include <GL/glew.h>

#include "mtlLoader.hpp"
#include "image.hpp"

using namespace std;

Material* loadMTL(istream &mtl) {
  glm::vec3 diffuseColor;
  glm::vec3 specularColor;
  GLuint texture;
  ShaderType shader;

  while (!mtl.eof()) {
    string line;
    getline(mtl, line);
    if (mtl.fail()) {
      cout << "Read failed." << endl;
      throw exception();
    }
    if (line.size()!=0) {
      int headEnd = line.find_first_of(" ");
      if (headEnd == string::npos) {
        cout << "No header..." << endl;
      }
      else {
        string header = line.substr(0,headEnd);
        if (header == "Kd"){
          float r,g,b;
          int tmp = line.find_first_of(" ",headEnd+1);
          r = stof(line.substr(headEnd+1,tmp));
          int tmp2 = line.find_first_of(" ",tmp+1);
          g = stof(line.substr(tmp+1,tmp2));
          tmp = line.find_first_of(" ",tmp2+1);
          b = stof(line.substr(tmp2+1,tmp));
          diffuseColor = glm::vec3 (r,g,b);
        }
        else if (header == "Ks") {
          float r,g,b;
          int tmp = line.find_first_of(" ",headEnd+1);
          r = stof(line.substr(headEnd+1,tmp));
          int tmp2 = line.find_first_of(" ",tmp+1);
          g = stof(line.substr(tmp+1,tmp2));
          tmp = line.find_first_of(" ",tmp2+1);
          b = stof(line.substr(tmp2+1,tmp));
          specularColor = glm::vec3 (r,g,b);
        }
        else if (header == "map_Kd") {
          int end = line.find_first_of("#",headEnd+1);
          texture = loadPNGAsTexture(line.substr(headEnd+1,end));
          shader = TextureShader;
        }
      }
    }
    mtl.peek();
  }

  Material* ret = new Material(diffuseColor,specularColor,texture,shader);
  return ret;
}


Material* loadMTLString(string mtlString) {
  istringstream mtlStream (mtlString);
  return loadMTL(mtlStream);
}

// Using deleted function?
/*Material* loadMTLFile(string mtlPath) {
  ifstream mtlFile (mtlPath.c_str());
  if (!mtlFile.good()) {
    cout << "File not found: " << mtlPath << endl;
    throw exception();
  }
  return loadMTL(mtlFile);
}*/
