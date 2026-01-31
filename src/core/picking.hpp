#ifndef PICKING_HPP
#define PICKING_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

#include "utils/logger.hpp"

class Picking {
 public:
  bool init(int width, int height);

  void enable_writing();
  void disable_writing();

  float read_pixel(unsigned int x, unsigned int y);

 private:
  GLuint m_fbo;
  GLuint m_picking_texture;
  GLuint m_depth_texture;
};

#endif