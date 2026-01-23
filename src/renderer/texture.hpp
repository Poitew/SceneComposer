#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <assimp/scene.h>
#include <assimp/texture.h>

#include <assimp/Importer.hpp>
#include <iostream>
#include <string>

class Texture {
 public:
  std::string type;

  Texture() = default;
  Texture(std::string imagepath, std::string type);
  Texture(unsigned char* memory, unsigned int length, std::string type);

  void use(GLenum text_num);
  GLuint get_texture();

  bool is_valid();

 private:
  GLuint texture = 0;
  int width;
  int height;
  int n_channels;

  void init_texture();
  void check_data(unsigned char* data);
  unsigned char* load_texture(std::string imagepath);
  unsigned char* load_texture_embeed(unsigned char* memory, int length);
};

#endif