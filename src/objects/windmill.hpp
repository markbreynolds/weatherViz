#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "object.hpp"

class Windmill : public Object {
protected:
  std::vector<unsigned short> groupIndex;
  float spinSpeed = 0.01;
  glm::vec3 groupCenter;
  void updateGroupCenter();
public:
  Windmill() : Object() {};
  Windmill(glm::vec3 position, glm::quat rotation) : Object(position, rotation) {};
  Windmill(std::vector <glm::vec3> vertices, std::vector <glm::vec2> uvs, std::vector <glm::vec3> normals, std::vector <unsigned short> index, Material* mat) : Object(vertices,uvs,normals,index,mat) {};
  Windmill(std::vector <glm::vec3> vertices, std::vector <glm::vec2> uvs, std::vector <glm::vec3> normals, std::vector <unsigned short> index, Material* mat, std::vector <unsigned short> indexedGroup) : Object(vertices,uvs,normals,index,mat), groupIndex(indexedGroup) {updateGroupCenter();};
  void update(float tick);
  void setSpinSpeed(float speed);
  float getSpinSpeed();
};
