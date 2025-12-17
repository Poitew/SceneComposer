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
  glfwSetKeyCallback(window, Keyboard::key_callback);

  shader = {"shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"};
  shader.use();

  camera = {90.0f, width, height, 0.1f, 1000.0f};

  shader.set_mat4("projection", camera.get_projection());
  shader.set_mat4("view", camera.get_view());

  return true;
}

void Engine::begin_frame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  float current = glfwGetTime();
  Time::update(current);

  camera.move(Keyboard::is_down(GLFW_KEY_W), Keyboard::is_down(GLFW_KEY_S),
              Keyboard::is_down(GLFW_KEY_A), Keyboard::is_down(GLFW_KEY_D),
              Keyboard::is_down(GLFW_KEY_E), Keyboard::is_down(GLFW_KEY_Q), Time::delta());

  shader.set_mat4("view", camera.get_view());
}

void Engine::end_frame() {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

Shader& Engine::get_shader() { return shader; }

bool Engine::should_close() { return glfwWindowShouldClose(window); }

Engine::~Engine() { glfwTerminate(); }
