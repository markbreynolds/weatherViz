#include "objLoader.hpp"

using namespace std;

bool loadOBJ(istream &obj, istream &mtl, vector<glm::vec3> &vertices, vector<glm::vec2> &uvs, vector<glm::vec3> &normals, Material** material) {
  vector <glm::vec3> vertexList;
  vector <glm::vec2> uvList;
  vector <glm::vec3> normalList;

  while (!obj.eof()) {
    string line;
    getline(obj, line);
    if (obj.fail()) {
      cout << "Read failed." << endl;
      return false;
    }

    int headEnd = line.find_first_of(" ");
    if (headEnd == string::npos) {
      cout << "No header..." << endl;
    }
    else {
      string header = line.substr(0,headEnd);
      if (header == "v"){
        float x,y,z;
        int tmp = line.find_first_of(" ",headEnd+1);
        x = stof(line.substr(headEnd+1,tmp));
        int tmp2 = line.find_first_of(" ",tmp+1);
        y = stof(line.substr(tmp+1,tmp2));
        tmp = line.find_first_of(" ",tmp2+1);
        z = stof(line.substr(tmp2+1,tmp));
        glm::vec3 vertex (x,y,z);
        vertexList.push_back(vertex);
      }
      else if (header == "vt") {
        float x,y;
        int tmp = line.find_first_of(" ",headEnd+1);
        x = stof(line.substr(headEnd+1,tmp));
        int tmp2 = line.find_first_of(" ",tmp+1);
        //if (tmp2 == -1) {cout << '.';}
        y = stof(line.substr(tmp+1,tmp2));
        if (OBJ_UV_FLIPY) {
          y = 1-y;
        }
        glm::vec2 uv (x,y);
        uvList.push_back(uv);
      }
      else if (header == "vn") {
        float x,y,z;
        int tmp = line.find_first_of(" ",headEnd+1);
        x = stof(line.substr(headEnd+1,tmp));
        int tmp2 = line.find_first_of(" ",tmp+1);
        y = stof(line.substr(tmp+1,tmp2));
        tmp = line.find_first_of(" ",tmp2+1);
        z = stof(line.substr(tmp2+1,tmp));
        glm::vec3 normal (x,y,z);
        normalList.push_back(normal);
      }
      else if (header == "f"){
        int start, end;
        start = headEnd+1;
        end = line.find_first_of(" ",start);
        while (start!=0)
        {
          int vertex,uv,normal;

          int tmp = line.find_first_of("/",start);
          string sub = line.substr(start,tmp-start);
          if (sub.size()==0){
            cout << "Face is missing vertex." << endl;
            return false;
          }
          else {
            vertex = stoi(sub);
          }

          int tmp2 = line.find_first_of("/",tmp+1);
          sub = line.substr(tmp+1,tmp2-tmp-1);
          if (sub.size()>0)
          {
            uv = stoi(sub);
          }
          else {
            uv = -1;
          }

          sub = line.substr(tmp2+1,end-tmp2-1);
          if (sub.size()==0)
          {
            cout << "Vertex is missing normal." << endl;
            return false;
          }
          else {
            normal = stof(sub);
          }

          /*cout << "V" << vertex << ": [" << vertexList[vertex-1][0] << ", " << vertexList[vertex-1][1] << ", " << vertexList[vertex-1][2] << "]" << endl;
          cout << "N" << normal << ": [" << normalList[normal-1][0] << ", " << normalList[normal-1][1] << ", " << normalList[normal-1][2] << "]" << endl;
          string input;
          cin >> input; //*/

          vertices.push_back(vertexList[vertex-1]);
          if (uv!=-1) {
            uvs.push_back(uvList[uv-1]);
          }
          normals.push_back(normalList[normal-1]);
          start = end+1;
          end = line.find_first_of(" ",start);
        }
      }
    }
    obj.peek();
  }

  Material* materialTest = loadMTL(mtl);
  *material = materialTest;

  return true;
}

bool loadOBJString(string objString, string mtlString, vector<glm::vec3> &vertices, vector<glm::vec2> &uvs, vector<glm::vec3> &normals, Material** material) {
  istringstream objStream (objString);
  istringstream mtlStream (mtlString);
  return loadOBJ(objStream, mtlStream, vertices, uvs, normals, material);
}

bool loadOBJFile(const string path, vector<glm::vec3> &vertices, vector<glm::vec2> &uvs, vector<glm::vec3> &normals, Material** material) {
  string mtlPath = path.substr(0,path.size()-3)+"mtl";
  return loadOBJFile(path, mtlPath, vertices, uvs, normals, material);
}

bool loadOBJFile(string objPath, string mtlPath, vector<glm::vec3> &vertices, vector<glm::vec2> &uvs, vector<glm::vec3> &normals, Material** material) {
  ifstream objFile (objPath.c_str());
  ifstream mtlFile (mtlPath.c_str());
  if (!objFile.good()) {
    cout << "File not found: " << objPath << endl;
    return false;
  }
  if (!mtlFile.good()) {
    cout << "File not found: " << mtlPath << endl;
    return false;
  }
  return loadOBJ(objFile, mtlFile, vertices, uvs, normals, material);
}
