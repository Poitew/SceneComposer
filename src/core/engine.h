#ifndef ENGINE_H
#define ENGINE_H

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

#include "renderer/shader.h"

class Engine {
 private:
  GLFWwindow* window;
  int width;
  int height;
  const char* window_name;

  Shader shader;

 public:
  Engine(int width, int height, const char* window_name);
  ~Engine();

  bool init_application();
  void draw();
  bool should_close();
};

#endif