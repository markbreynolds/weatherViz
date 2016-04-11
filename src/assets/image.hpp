#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <lodepng.h>

/** @defgroup image image.hpp
 * Code for working with pngs.
 * @{
 */

/**
 * Prints to standard out the height and width of a png image.
 * @param imagePath The path to the image.
 */
void pngInfo(std::string imagePath);

/**
 * Loads a png file into an OpenGL texture.
 *
 * @pre GLFW must be initialized.
 *
 * @param imagePath The path to the image.
 */
GLuint pngTexture(std::string imagePath);

/** @} */
