#include "object.hpp"

#include <iostream>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "assets/shaders.hpp"

Object::Object() { bindBuffers(); }

Object::Object(glm::vec3 position, glm::quat rotation) : position(position), rotation(rotation) { bindBuffers(); }
Object::Object(std::vector <glm::vec3> vertices, std::vector <glm::vec2> uvs, std::vector <glm::vec3> normals, std::vector <unsigned short> index, Material* mat) : vertexIndex(vertices), uvIndex(uvs), normalIndex(normals), indices(index), material(mat) { bindBuffers(); }
Object::Object(std::vector <glm::vec3> vertices, std::vector <glm::vec2> uvs, std::vector <glm::vec3> normals, std::vector <unsigned short> index, Material* mat, glm::vec3 position, glm::quat rotation) : vertexIndex(vertices), uvIndex(uvs), normalIndex(normals), indices(index), material(mat), position(position), rotation(rotation) { bindBuffers(); }

void Object::bindBuffers() {
  glGenBuffers(1,&vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,vertexIndex.size() * sizeof(glm::vec3),&vertexIndex[0],GL_STATIC_DRAW);

	glGenBuffers(1,&uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
	glBufferData(GL_ARRAY_BUFFER,uvIndex.size() * sizeof(glm::vec3),&uvIndex[0],GL_STATIC_DRAW);

	glGenBuffers(1,&normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glBufferData(GL_ARRAY_BUFFER,normalIndex.size() * sizeof(glm::vec3),&normalIndex[0],GL_STATIC_DRAW);

	glGenBuffers(1,&elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

Object::~Object() {
  glDeleteBuffers(1,&vertexBuffer);
  glDeleteBuffers(1,&uvBuffer);
	glDeleteBuffers(1,&normalBuffer);
  glDeleteBuffers(1,&elementBuffer);
}

void Object::translate(glm::vec3 amt) {
  position += amt;
}

void Object::rotate(glm::quat amt) {
  rotation = amt * rotation;
}

void Object::rotate(glm::vec3 amt) {
  rotation = glm::quat(amt) * rotation;
}

ShaderType Object::getShaderType() {
  return material->getShaderType();
}

void Object::update(float tick) {
  return;
}

void Object::draw(GLuint shader, glm::mat4 view, glm::mat4 projection) {
  if (material->getShaderType() == TextureShader) {
    GLuint textureID = glGetUniformLocation(shader, TEXTURE_DIFFUSE_SAMPLER_UNIFORM_LOCATION);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->getDiffuseTexture());
    glUniform1i(textureID, 0);
  }
  else {
    GLuint matColorID = glGetUniformLocation(shader, SOLID_DIFFUSE_COLOR_UNIFORM_LOCATION);
    glm::vec3 matColor = material->getDiffuseColor();
    glUniform3f(matColorID,matColor.r,matColor.g,matColor.b);
  }	 //*/

  GLuint mvpID = glGetUniformLocation(shader,"mvp");

  GLuint modelID = glGetUniformLocation(shader,"m");
  GLuint viewID = glGetUniformLocation(shader,"v");

  //glm::mat4 rotationMat = glm::mat4_cast(rotation);
  glm::mat4 model = glm::translate(position) * glm::mat4_cast(rotation);
  glm::mat4 mvp = projection * view * model;

  glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0

  );
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0
  );

  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
    2,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_SHORT,(void*)0);
  //glDrawArrays(GL_TRIANGLES,0,vertices.size());
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
