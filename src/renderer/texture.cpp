#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"

#include <stb/stb_image.h>

unsigned char* Texture::load_texture_embeed(unsigned char* memory, int length) {
  unsigned char* data = nullptr;

  data = stbi_load_from_memory(memory, length, &width, &height, &n_channels, 0);

  return data;
}

unsigned char* Texture::load_texture(std::string imagepath) {
  unsigned char* data = nullptr;

  std::string directory = imagepath.substr(0, imagepath.find_last_of("/\\"));
  std::string absolute_path = directory + '/' + imagepath.c_str();

  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(absolute_path.c_str(), &width, &height, &n_channels, 0);

  return data;
}

void Texture::init_texture() {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Texture::check_data(unsigned char* data) {
  if (data) {
    GLenum format = (n_channels == 4) ? GL_RGBA : GL_RGB;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture \n";
  }
}

Texture::Texture(std::string imagepath, std::string type) : type{type} {
  init_texture();

  unsigned char* data = load_texture(imagepath);

  check_data(data);

  stbi_image_free(data);
}

Texture::Texture(unsigned char* memory, unsigned int length, std::string type) : type{type} {
  init_texture();

  unsigned char* data = load_texture_embeed(memory, length);

  check_data(data);

  stbi_image_free(data);
}

void Texture::use(GLenum text_num) {
  glActiveTexture(text_num);
  glBindTexture(GL_TEXTURE_2D, texture);
}

GLuint Texture::get_texture() { return texture; }