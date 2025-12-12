#include "engine.h"

Engine::Engine(int width, int height, const char* window_name)
    : width{width}, height{height}, window_name{window_name} {}

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

  std::string vert_shader = "shaders/vertex_shader.glsl";
  std::string frag_shader = "shaders/fragment_shader.glsl";
  shader = {vert_shader, frag_shader};

  return true;
}

void Engine::draw() {
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool Engine::should_close() { return glfwWindowShouldClose(window); }

Engine::~Engine() { glfwTerminate(); }
