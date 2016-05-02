#pragma once

#include <vector>
#include <queue>

#include <glm/glm.hpp>

/** @defgroup vboIndexer vboIndexer.hpp
 * Code for creating an indexed VBO from unindexed data.
 * @{
 */

/**
 * Converts unindexed data into an indexed VBO.
 *
 * @pre vertices.size() == normals.size() && (uvs.size() == 0 || uvs.size() == vertices.size())
 *
 * @param vertices List of vertices from the unindexed data.
 * @param uvs List of UVs from the unindexed data.
 * @param normals List of normals from the unindexed data.
 * @param index List of indices representing vertices in the indexed VBO.
 * @param vertexIndex List of indexed vertices.
 * @param uvIndex List of indexed UVs.
 * @param normalIndex List of indexed normals.
 */
void createVBOIndex(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, std::vector<unsigned short> &indices, std::vector<glm::vec3> &vertexIndex, std::vector<glm::vec2> &uvIndex, std::vector<glm::vec3> &normalIndex);

/**
 * @overload
 *
 * @param groupIndex Group indices input.
 * @param indexedGroup Group indices output.
 */
void createVBOIndex(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, std::queue<unsigned short> &groupIndex, std::vector<unsigned short> &indices, std::vector<unsigned short> &indexedGroup, std::vector<glm::vec3> &vertexIndex, std::vector<glm::vec2> &uvIndex, std::vector<glm::vec3> &normalIndex);
/** @} */
