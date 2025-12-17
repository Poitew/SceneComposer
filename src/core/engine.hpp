#ifndef ENGINE_HPP
#define ENGINE_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/matrix.hpp>
#include <string>

#include "core/camera.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "renderer/mesh.hpp"
#include "renderer/shader.hpp"
#include "utils/time.hpp"

class Engine {
 public:
  Engine(int width, int height, const char* window_name);
  ~Engine();

  bool init_application();
  bool init_imgui();
  void begin_frame();
  void end_frame();
  bool should_close();

  Shader& get_shader();

 private:
  GLFWwindow* window;
  int width;
  int height;
  const char* window_name;

  Shader shader;
  Camera camera;

  static void fb_size_callback(GLFWwindow* window, int width, int height);
};

#endif