#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

/** @defgroup material material.hpp
 * Code for describing how meshes should colored in the fragment shader..
 * @{
 */

/// Which fragment shader should OpenGL use for this material.
enum ShaderType { SolidShader, TextureShader, SkyShader};

class Material {
private:
  glm::vec3 diffuseColor;
  glm::vec3 specColor;
  GLuint diffuseTexture;
  ShaderType shader;
public:

  /// Default Constructor
  Material() : diffuseColor(glm::vec3(0,0,0)), specColor(glm::vec3(255,255,255)), diffuseTexture(0), shader(SolidShader) {}

  /// Solid Constructor
  Material(glm::vec3 diffuse, glm::vec3 specular) : diffuseColor(diffuse), specColor(specular), diffuseTexture(0), shader(SolidShader) {}

  /// Texture Constructor
  Material(GLuint texture, ShaderType shader) : diffuseColor(glm::vec3(0,0,0)), specColor(glm::vec3(255,255,255)), diffuseTexture(texture), shader(shader) {}

  /// Explicit Constructor
  Material(glm::vec3 diffuse, glm::vec3 specular, GLuint texture, ShaderType shader) : diffuseColor(diffuse), specColor(specular), diffuseTexture(texture), shader(shader) {}

  /// Sets what solid color this material should be.
  void setDiffuseColor(glm::vec3 color) {diffuseColor = color;}

  /// Sets what color the hightlights of material should be.
  void setSpecularColor(glm::vec3 color) {specColor = color;}

  /// Sets what kind of shader should be used to display this material.
  void setShaderType(ShaderType newShader) {shader = newShader;}

  /// Sets the texture ID representing the diffuse texture for this material.
  void setDiffuseTexture(GLuint texture) {diffuseTexture = texture;}

  /// Returns what solid color this material is.
  glm::vec3 getDiffuseColor() const {return diffuseColor;}

  /// Returns what color the hightlights of material are.
  glm::vec3 getSpecularColor() const {return specColor;}

  /// Returns what kind of shader should be used to display this material.
  ShaderType getShaderType() const {return shader;}

  /// Returns the texture ID representing the diffuse texture for this material.
  GLuint getDiffuseTexture() const {return diffuseTexture;}
};

/** @} */
