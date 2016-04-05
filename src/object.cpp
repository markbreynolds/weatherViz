#include "object.hpp"

bool loadOBJ(istream &stream, vector<glm::vec3> &vertices, vector<glm::vec3> &normals) {
  vector <glm::vec3> vertexList;
  vector <glm::vec3> normalList;

  while (!stream.eof()) {
    string line;
    getline(stream, line);
    if (stream.fail()) {
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
      if (header == "vn"){
        float x,y,z;
        int tmp = line.find_first_of(" ",headEnd+2);
        x = stof(line.substr(headEnd+2,tmp));
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

          sub = line.substr(tmp2+1,end-tmp2-1);
          if (sub.size()==0)
          {
            cout << "Vertex is missing normal." << endl;
            return false;
          }
          else {
            normal = stof(sub);
          }

          vertices.push_back(vertexList[vertex-1]);
          normals.push_back(normalList[normal-1]);
          start = end+1;
          end = line.find_first_of(" ",start);
        }
      }
    }
    stream.peek();
  }

  return true;
}

bool loadOBJString(string String, vector<glm::vec3> &vertices, vector<glm::vec3> &normals) {
  istringstream stream (String);
  return loadOBJ(stream, vertices, normals);
}

bool loadOBJFile(const string path, vector<glm::vec3> &vertices, vector<glm::vec3> &normals) {
  ifstream file (path.c_str());
  if (!file.good()){
    cout << "File not found." << endl;
    return false;
  }
  return loadOBJ(file, vertices, normals);
}
