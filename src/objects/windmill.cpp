#include "windmill.hpp"

#include <iostream>

#include <glm/gtc/quaternion.hpp>

void Windmill::setSpinSpeed(float speed) {
  spinSpeed = speed;
}
float Windmill::getSpinSpeed() {
  return spinSpeed;
}

void Windmill::updateGroupCenter() {
  glm::vec3 sum;
  for (int i=0; i<groupIndex.size(); i++){
    sum+=vertexIndex[groupIndex[i]];
  }
  groupCenter = glm::vec3(sum.x/groupIndex.size(),sum.y/groupIndex.size(),sum.z/groupIndex.size());
}

void Windmill::update(float tick) {
  for (int i=0; i<groupIndex.size(); i++){
    glm::quat rotation = glm::quat(glm::vec3(spinSpeed,0,0));
    //glm::quat rotation = glm::quat(glm::vec3(spinSpeed,spinSpeed,spinSpeed));   // For more fun rotation...
    unsigned short index = groupIndex[i];
    vertexIndex[index] = groupCenter + (rotation * (vertexIndex[index]-groupCenter));
    normalIndex[index] = rotation * normalIndex[index];
  }
  glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
  glBufferSubData(GL_ARRAY_BUFFER,0,vertexIndex.size() * sizeof(glm::vec3),&vertexIndex[0]);
  glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
  glBufferSubData(GL_ARRAY_BUFFER,0,normalIndex.size() * sizeof(glm::vec3),&normalIndex[0]);
}
