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

  glEnable(GL_DEPTH_TEST);

  Mouse::initialize(width, height);

  shader = {"shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"};
  shader.use();

  picking_shader = {"shaders/picking_vertex.glsl", "shaders/picking_fragment.glsl"};
  picking_buffer.init(width, height);

  camera = {90.0f, width, height, 0.1f, 1000.0f};

  shader.set_mat4("projection", camera.get_projection());
  shader.set_mat4("view", camera.get_view());
  shader.set_float("lightIntensity", 1.0f);

  skybox = {"src/core/skybox",
            {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"}};

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
  ImGui::NewFrame();

  glViewport(0, 0, width, height);
  glClearColor(0.24f, 0.24f, 0.24f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Time::update(glfwGetTime());

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera.rotate(Mouse::get_delta_x(), Mouse::get_delta_y(), Time::delta());
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  camera.move(Keyboard::is_down(GLFW_KEY_W), Keyboard::is_down(GLFW_KEY_S),
              Keyboard::is_down(GLFW_KEY_A), Keyboard::is_down(GLFW_KEY_D),
              Keyboard::is_down(GLFW_KEY_E), Keyboard::is_down(GLFW_KEY_Q), Time::delta());

  shader.use();
  shader.set_mat4("view", camera.get_view());
  shader.set_mat4("projection", camera.get_projection());

  picking_shader.use();
  picking_shader.set_mat4("view", camera.get_view());
  picking_shader.set_mat4("projection", camera.get_projection());
}

unsigned int Engine::read_click() {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int pixelX = (int)xpos;
    int pixelY = height - (int)ypos;

    float pixel_ID = picking_buffer.read_pixel(pixelX, pixelY);

    if (pixel_ID > 0.0f) {
      unsigned int clicked_ID = (unsigned int)pixel_ID;
      return clicked_ID;
    }
  }

  return 0;
}

void Engine::begin_picking() {
  picking_buffer.enable_writing();
  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::close_picking() {
  picking_buffer.disable_writing();
  glClear(GL_DEPTH_BUFFER_BIT);
}

void Engine::draw_skybox() { skybox.draw(camera.get_projection(), camera.get_view()); }

void Engine::draw_picker_gui(Transform& transform) {
  ImGui::Begin("Current object");

  static bool lock_scale = true;

  ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
  ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.5f);

  ImGui::Checkbox("Lock Axis", &lock_scale);

  if (lock_scale) {
    if (ImGui::DragFloat("Scale", &transform.scale.x, 0.01f)) {
      transform.scale.y = transform.scale.x;
      transform.scale.z = transform.scale.x;
    }
  } else {
    ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
  }

  if (ImGui::Button("Reset Transform")) {
    transform.position = glm::vec3(0.0f);
    transform.rotation = glm::vec3(0.0f);
    transform.scale = glm::vec3(1.0f);
  }

  ImGui::End();
}

void Engine::end_frame() {
  Mouse::reset_deltas();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
  glfwPollEvents();
}

Shader& Engine::get_shader() { return shader; }

Shader& Engine::get_picking_shader() { return picking_shader; }

bool Engine::should_close() { return glfwWindowShouldClose(window); }

Engine::~Engine() {
  glfwTerminate();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
