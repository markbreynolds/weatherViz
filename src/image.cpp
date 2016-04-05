#include "image.hpp"

void pngInfo(string imageFile) {
  vector<unsigned char> image;
  unsigned width, height;

  unsigned error = lodepng::decode(image, width, height, imageFile);

  cout << "File: " << imageFile << endl;
  cout << "Width: " << width << endl;
  cout << "Height: " << height << endl;
}

GLuint pngTexture(string imageFile) {
  vector<unsigned char> image;
  unsigned width, height;

  unsigned error = lodepng::decode(image,width,height, imageFile);

  if (error != 0) {
    cout << "error " << error << ": " << lodepng_error_text(error) << endl;
    return 1;
  }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  return textureID;
}
