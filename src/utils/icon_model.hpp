#ifndef ICON_MODEL_HPP
#define ICON_MODEL_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/matrix.hpp>
#include <string>

#include "renderer/shader.hpp"
#include "renderer/texture.hpp"

class IconModel {
 public:
  IconModel() = default;
  IconModel(std::string icon_path);
  void draw(glm::mat4& view, glm::mat4& projection);

  void set_model_matrix(glm::mat4& matrix);

 private:
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

  glm::mat4 model;
  Shader icon_shader;
  Texture icon_texture;
};

#endif