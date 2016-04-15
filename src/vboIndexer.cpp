#include <string.h>
#include <map>
#include <iostream>

#include "vboIndexer.hpp"

struct PackedVertex{
  glm::vec3 pos;
  glm::vec2 uv;
  glm::vec3 normal;
  bool operator < (const PackedVertex that) const {
    return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) < 0;
  }
};

unsigned short pvToIndex(PackedVertex pv, std::map<PackedVertex, unsigned short> indexMap) {
  std::map<PackedVertex, unsigned short>::iterator it = indexMap.find(pv);
  if (it==indexMap.end()) {
    return -1;
  }
  else {
    unsigned short ret = it->second;
    return ret;
  }
}

void createVBOIndex(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, std::vector<unsigned short> &indices, std::vector<glm::vec3> &vertexIndex, std::vector<glm::vec2> &uvIndex, std::vector<glm::vec3> &normalIndex) {
  bool uvsPresent;
  if (uvs.size() == 0) { uvsPresent = false; }
  else { uvsPresent = true; }

  std::map<PackedVertex, unsigned short> indexMap;

  for (unsigned int i=0; i < vertices.size(); i++) {
    glm::vec2 uv;
    if (uvsPresent) { uv = uvs[i]; }
    else { uv = glm::vec2(0,0); }

    PackedVertex pv = {vertices[i],uv,normals[i]};
    unsigned short index = pvToIndex(pv,indexMap);
    //std::cout << index << std::endl;
    if (index==(unsigned short)-1) {
      vertexIndex.push_back(vertices[i]);
      uvIndex.push_back(uv);
      normalIndex.push_back(normals[i]);
      unsigned short newIndex = (unsigned short)vertexIndex.size()-1;
      indices.push_back(newIndex);
      indexMap[pv] = newIndex;
    }
    else {
      indices.push_back(index);
    }
  }
}
