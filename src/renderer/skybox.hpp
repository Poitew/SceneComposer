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

class Skybox {
 public:
  Skybox() = default;
  Skybox(std::string dir, std::vector<std::string> faces);
  void draw(glm::mat4 projection, glm::mat4 view);

 private:
  GLuint textureID;
  GLuint VAO;
  GLuint VBO;
  Shader skybox_shader;

  std::vector<GLfloat> skybox_vertices;
};

#endif