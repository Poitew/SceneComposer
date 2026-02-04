#ifndef RENDERER_HPP
#define RENDERER_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

#include "utils/logger.hpp"

class Renderer {
 public:
  Renderer() = default;
  Renderer(int width, int height);
  void enable_writing();
  void disable_writing();
  void save_to_file();

 private:
  GLuint FBO;
  GLuint RBO;
  GLuint texture;

  int width;
  int height;
};

#endif