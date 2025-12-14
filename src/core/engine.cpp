#include "engine.hpp"

Engine::Engine(int width, int height, const char* window_name)
    : width{width}, height{height}, window_name{window_name} {}

void Engine::fb_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

bool Engine::init_application() {
  if (!glfwInit()) {
    return false;
  }

  window = glfwCreateWindow(width, height, window_name, NULL, NULL);
  if (!window) {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGL(glfwGetProcAddress)) {
    glfwTerminate();
    return false;
  }

  glfwSetFramebufferSizeCallback(window, fb_size_callback);

  shader = {"shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"};
  shader.use();

  camera = {90, width, height, 0.1f, 100.0f};

  shader.set_mat4("projection", camera.get_projection());
  shader.set_mat4("view", camera.get_view());

  return true;
}

void Engine::begin_frame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO: real input with future Keyboard class
  camera.move(true, false, false, false, false, false, 0.01f);

  shader.set_mat4("view", camera.get_view());
}

void Engine::end_frame() {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool Engine::should_close() { return glfwWindowShouldClose(window); }

Engine::~Engine() { glfwTerminate(); }
