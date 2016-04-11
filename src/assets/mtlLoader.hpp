#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

#include <material.hpp>

/** @defgroup mtlLoader mtlLoader.hpp
 * Code for loading .obj files.
 * @{
 */

/**
 * Loads a .mtl from a stream and creates a Material object.
 *
 * @pre Stream must contain only one material.
 *
 * @param mtl A stream containing the .mtl data.
 *
 * @return Returns a Material object with properties loaded from the .mtl stream.
 */
Material* loadMTL(std::istream &mtl);

/**
 * Loads a .mtl from a string.
 *
 * @pre String must contain only one material.
 *
 * @param mtlString A string containing the .mtl data.
 *
 * @return Returns a Material object with properties loaded from the .mtl string.
 */
Material* loadMTLString(std::string mtlString);


/**
 * Loads a .mtl from a file.
 *
 * @pre File must contain only one material.
 *
 * @param mtlPath A string representing the path to the mtl file.
 *
 * @return Returns a Material object with properties loaded from the .mtl file.
 */
Material* loadMTLFile(std::string mtlPath);

/** @} */
