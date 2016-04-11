#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

#include "mtlLoader.hpp"

/** @defgroup objLoader objLoader.hpp
 * Code for loading .obj files.
 * @{
 */

/// Whether or not to flip UVs along the X axis.
#define OBJ_UV_FLIPY true

/**
 * Loads an .obj and .mtl from two streams and places the appropiate
 * data into the appropiate vectors.
 *
 * @pre All faces in the obj must be triangles and each vertex must
 *        have a normal. Stream must contain only one obj.
 * @post vertices.size()==normals.size()
 *
 * @param obj A stream containing the .obj data.
 * @param mtl A stream containing the .mtl data.
 * @param vertices A vector that loaded vertices will be put into.
 * @param uvs A vector that loaded UVs will be put into.
 * @param normals A vector that loaded normals will be put into.
 * @param material A Material pointer that will point to the Material.
 *
 * @return @c true if succeeds, false otherwise.
 *
 * @see loadMTL()
 */
bool loadOBJ(std::istream &obj,  std::istream &mtl, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, Material** material);

/**
 * Loads an .obj and .mtl from two strings and places the appropiate data into the appropiate vectors.
 *
 * Mainly for testing purposes.
 *
 * @param obj A string containing the .obj data.
 * @param mtl A string containing the .mtl data.
 *
 * @see loadOBJ().
 */
bool loadOBJString(std::string objString, std::string mtlString, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, Material** material);

/**
 * Loads an .obj and .mtl from two files and places the appropiate data into the
 *  appropiate vectors. Files must be in form of "fileName.obj" and "fileName.mtl".
 *  Otherwise, the other loadOBJFile function must be used.
 *
 * @param path The path to the .obj file. @c path must include ".obj".
 *
 * @see loadOBJFile(std::string objPath, std::string mtlPath, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals)
 * @see loadOBJ().
 */
bool loadOBJFile(std::string path, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, Material** material);

/**
 * Loads an .obj and .mtl from two files and places the appropiate data into the appropiate vectors.
 *
 * @param objPath The path to the obj file.
 * @param mtlPath The path to the mtl file.
 *
 * @see loadOBJ().
 */
bool loadOBJFile(std::string objPath, std::string mtlPath, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, Material** material);

/** @} */
