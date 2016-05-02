#pragma once

#include <vector>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>

#include "material.hpp" // src/assets/material.hpp

class Object {
protected:
  GLuint vertexBuffer;
  GLuint uvBuffer;
  GLuint normalBuffer;
  GLuint elementBuffer;
  std::vector<glm::vec3> vertexIndex;
  std::vector<glm::vec2> uvIndex;
  std::vector<glm::vec3> normalIndex;
  std::vector<unsigned short> indices;
  Material* material;
  glm::vec3 position;
  glm::quat rotation;
  void bindBuffers();
public:
  Object();
  Object(glm::vec3 position, glm::quat rotation);
  Object(std::vector <glm::vec3> vertices, std::vector <glm::vec2> uvs, std::vector <glm::vec3> normals, std::vector <unsigned short> index, Material* mat);
  Object(std::vector <glm::vec3> vertices, std::vector <glm::vec2> uvs, std::vector <glm::vec3> normals, std::vector <unsigned short> index, Material* mat, glm::vec3 position, glm::quat rotation);
  ~Object();
  void setPosition(glm::vec3 pos);
  void translate(glm::vec3 amt);
  void rotate(glm::quat amt);
  void rotate(glm::vec3 amt);
  ShaderType getShaderType();
  virtual void update(float tick) {};
  void draw(GLuint shader, glm::mat4 view, glm::mat4 projection);
};
