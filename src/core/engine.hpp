#ifndef ENGINE_HPP
#define ENGINE_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <nfd.h>

#include <glm/matrix.hpp>
#include <string>
#include <vector>

#include "core/camera.hpp"
#include "core/picking.hpp"
#include "core/vr_context.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "renderer/mesh.hpp"
#include "renderer/model.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/skybox.hpp"
#include "utils/icon_model.hpp"
#include "utils/time.hpp"

using CScene = std::unordered_map<unsigned int, std::shared_ptr<Model>>;

class Engine {
 public:
  Engine(int width, int height, const char* window_name, bool vr_mode);
  ~Engine();

  bool init_application();
  bool init_imgui();

  void begin_frame();

  void begin_picking();
  void close_picking();

  void begin_render();
  void close_render();

  void begin_vr_frame();
  bool next_eye();
  void end_vr_frame();

  void draw_skybox(glm::mat4 view, glm::mat4 proj);
  void draw_icons(glm::mat4 view, glm::mat4 proj);

  void update_shaders(glm::mat4& view, glm::mat4 proj);

  void draw_object_properties_panel(Transform& transform, bool& hidden);
  void draw_world_properties_panel();
  void draw_main_bar(std::string& model_path, std::string& sky_path, bool& start_rendering);
  void draw_hierarchy_gui(CScene& scene, unsigned int& selected_id);
  void draw_bottom_log_panel();

  void load_sky(std::string path);
  unsigned int read_click();
  void end_frame();

  bool should_close();

  glm::mat4 get_view();
  glm::mat4 get_proj();

  Shader& get_shader();
  Shader& get_picking_shader();

 private:
  GLFWwindow* window;
  int width;
  int height;
  const char* window_name;
  bool vr_mode;

  VRContext vr_context;
  Shader shader;
  Shader picking_shader;
  Picking picking_buffer;
  Camera camera;
  Skybox skybox;
  IconModel sun_icon;
  Renderer renderer;

  static void fb_size_callback(GLFWwindow* window, int width, int height);
};

#endif