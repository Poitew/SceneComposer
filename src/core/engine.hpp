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
#include "core/picking.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "renderer/mesh.hpp"
#include "renderer/model.hpp"
#include "renderer/shader.hpp"
#include "utils/time.hpp"

class Engine {
 public:
  Engine(int width, int height, const char* window_name);
  ~Engine();

  bool init_application();
  bool init_imgui();

  void begin_frame();
  void begin_picking();
  void close_picking();
  void draw_picker_gui(Transform& transform);
  unsigned int read_click();
  void end_frame();

  bool should_close();

  Shader& get_shader();
  Shader& get_picking_shader();

 private:
  GLFWwindow* window;
  int width;
  int height;
  const char* window_name;

  Shader shader;
  Shader picking_shader;
  Camera camera;
  Picking picking_buffer;

  static void fb_size_callback(GLFWwindow* window, int width, int height);
};

#endif