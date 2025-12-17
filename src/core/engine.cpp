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

  Mouse::initialize(width, height);

  shader = {"shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"};
  shader.use();

  camera = {90.0f, width, height, 0.1f, 1000.0f};

  shader.set_mat4("projection", camera.get_projection());
  shader.set_mat4("view", camera.get_view());

  glfwSetFramebufferSizeCallback(window, fb_size_callback);
  glfwSetKeyCallback(window, Keyboard::key_callback);
  glfwSetCursorPosCallback(window, Mouse::mouse_callback);

  return true;
}

bool Engine::init_imgui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplOpenGL3_Init();
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  return true;
}

void Engine::begin_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.24f, 0.24f, 0.24f, 1.0f);

  float current = glfwGetTime();
  Time::update(current);

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera.rotate(Mouse::get_delta_x(), Mouse::get_delta_y(), Time::delta());
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  camera.move(Keyboard::is_down(GLFW_KEY_W), Keyboard::is_down(GLFW_KEY_S),
              Keyboard::is_down(GLFW_KEY_A), Keyboard::is_down(GLFW_KEY_D),
              Keyboard::is_down(GLFW_KEY_E), Keyboard::is_down(GLFW_KEY_Q), Time::delta());

  shader.set_mat4("view", camera.get_view());
}

void Engine::end_frame() {
  Mouse::reset_deltas();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
  glfwPollEvents();
}

Shader& Engine::get_shader() { return shader; }

bool Engine::should_close() { return glfwWindowShouldClose(window); }

Engine::~Engine() {
  glfwTerminate();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
