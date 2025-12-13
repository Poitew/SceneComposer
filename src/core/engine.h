#ifndef ENGINE_H
#define ENGINE_H

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <assimp/Importer.hpp>
#include <string>

#include "renderer/mesh.h"
#include "renderer/shader.h"

class Engine {
 public:
  Engine(int width, int height, const char* window_name);
  ~Engine();

  bool init_application();
  void draw();
  bool should_close();

 private:
  GLFWwindow* window;
  int width;
  int height;
  const char* window_name;

  Shader shader;
  Mesh mesh;
  static Assimp::Importer importer;

  static void fb_size_callback(GLFWwindow* window, int width, int height);
};

#endif