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

  return true;
}

void Engine::being_frame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shader.use();
}

void Engine::end_frame() {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool Engine::should_close() { return glfwWindowShouldClose(window); }

Engine::~Engine() { glfwTerminate(); }
