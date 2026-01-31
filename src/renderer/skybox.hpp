#ifndef SKYBOX_HPP
#define SKYBOX_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <stb/stb_image.h>

#include <glm/matrix.hpp>
#include <string>
#include <vector>

#include "renderer/shader.hpp"
#include "utils/logger.hpp"

class Skybox {
 public:
  Skybox() = default;
  Skybox(std::string hdr_path);
  void draw(glm::mat4 projection, glm::mat4 view);

 private:
  GLuint textureID;
  GLuint VAO;
  GLuint VBO;
  GLuint FBO;
  GLuint RBO;
  Shader conversion_shader;
  Shader skybox_shader;

  static std::vector<GLfloat> skybox_vertices;

  void create_geometry();
  void create_cubemap();
  void load_and_convert_hdri(std::string& hdr_path);
  void convert_hdri(GLuint hdr);
};

#endif