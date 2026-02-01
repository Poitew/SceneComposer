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

  shader = {"shaders/mesh.vs", "shaders/mesh.fs"};
  picking_shader = {"shaders/picking.vs", "shaders/picking.fs"};

  picking_buffer.init(width, height);

  camera = {90.0f, width, height, 0.1f, 1000.0f};

  skybox = {"assets/sky.hdr"};

  glfwSetFramebufferSizeCallback(window, fb_size_callback);
  glfwSetKeyCallback(window, Keyboard::key_callback);
  glfwSetCursorPosCallback(window, Mouse::mouse_callback);

  if (NFD_Init() != NFD_OKAY) {
    Logger::log("Failed to init NFD: " + std::string{NFD_GetError()});
    return false;
  }

  return true;
}

bool Engine::init_imgui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplOpenGL3_Init();
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  return true;
}

void Engine::begin_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGuiID main_id{ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),
                                               ImGuiDockNodeFlags_PassthruCentralNode)};

  static bool first_time = true;
  if (first_time) {
    ImGuiID left, right, bottom;

    left = ImGui::DockBuilderSplitNode(main_id, ImGuiDir_Left, 0.17f, nullptr, &main_id);
    bottom = ImGui::DockBuilderSplitNode(main_id, ImGuiDir_Down, 0.2f, nullptr, &main_id);
    right = ImGui::DockBuilderSplitNode(main_id, ImGuiDir_Right, 0.15f, nullptr, &main_id);

    ImGui::DockBuilderDockWindow("Scene Hierarchy", right);
    ImGui::DockBuilderDockWindow("System Log", bottom);
    ImGui::DockBuilderDockWindow("Object Properties", left);
    ImGui::DockBuilderFinish(main_id);

    first_time = false;
  }

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
              Keyboard::is_down(GLFW_KEY_E), Keyboard::is_down(GLFW_KEY_Q),
              Keyboard::is_down(GLFW_KEY_LEFT_SHIFT), Time::delta());

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

void Engine::draw_skybox() {
  skybox.draw(camera.get_projection(), camera.get_view());
  glViewport(0, 0, width, height);
}

void Engine::draw_object_properties_panel(Transform& transform, bool& hidden) {
  ImGui::Begin("Object Properties");

  static bool lock_scale = true;

  ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
  ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.5f);

  if (lock_scale) {
    if (ImGui::DragFloat("Scale", &transform.scale.x, 0.01f)) {
      transform.scale.y = transform.scale.x;
      transform.scale.z = transform.scale.x;
    }
  } else {
    ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
  }

  ImGui::Checkbox("Lock Axis", &lock_scale);
  ImGui::SameLine();
  ImGui::Checkbox("Hide model", &hidden);

  if (ImGui::Button("Reset Transform")) {
    transform.position = glm::vec3(0.0f);
    transform.rotation = glm::vec3(0.0f);
    transform.scale = glm::vec3(1.0f);
  }

  ImGui::End();
}

void Engine::draw_main_bar(std::string& model_path, std::string& sky_path) {
  bool is_model_import_open = false;
  bool is_sky_import_open = false;

  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("Importer")) {
    if (ImGui::MenuItem("Import Model (obj, fbx, glb, gltf)")) {
      is_model_import_open = true;
    }

    if (ImGui::MenuItem("Import Sky (hdr)")) {
      is_sky_import_open = true;
    }

    if (ImGui::MenuItem("Reset Sky")) {
      sky_path = "assets/sky.hdr";
    }

    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();

  nfdu8char_t* out_path = nullptr;
  const int FILTER_COUNT = 1;
  nfdopendialogu8args_t args = {0};

  if (is_model_import_open) {
    nfdu8filteritem_t filters[FILTER_COUNT] = {{"Models", "obj,fbx,glb,gltf"}};
    args.filterList = filters;
    args.filterCount = FILTER_COUNT;

    if (NFD_OpenDialogU8_With(&out_path, &args) == NFD_OKAY) {
      model_path = out_path;
      NFD_FreePathU8(out_path);
    }
  }
  if (is_sky_import_open) {
    nfdu8filteritem_t filters[FILTER_COUNT] = {{"Sky", "hdr"}};
    args.filterList = filters;
    args.filterCount = FILTER_COUNT;

    if (NFD_OpenDialogU8_With(&out_path, &args) == NFD_OKAY) {
      sky_path = out_path;
      NFD_FreePathU8(out_path);
    }
  }
}

void Engine::draw_hierarchy_gui(CScene& scene, unsigned int& selected_id) {
  ImGui::Begin("Scene Hierarchy");

  static ImGuiTreeNodeFlags root_flags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesToNodes;

  if (ImGui::TreeNodeEx("Scene", root_flags)) {
    for (auto& model : scene) {
      // The label id ## doesn't work
      std::string label = model.second->get_name() + "##" + std::to_string(model.first);

      ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf;

      if (selected_id == model.first) {
        leaf_flags |= ImGuiTreeNodeFlags_Selected;
      }

      ImGui::TreeNodeEx(label.c_str(), leaf_flags);

      if (ImGui::IsItemClicked()) {
        selected_id = model.first;
      }

      ImGui::TreePop();
    }

    ImGui::TreePop();
  }

  ImGui::End();
}

void Engine::draw_bottom_log_panel() {
  ImGui::Begin("System Log");
  auto logs = Logger::get_logs();

  if (ImGui::Button("Clear")) {
    logs.clear();
  }
  ImGui::SameLine();
  if (ImGui::Button("Copy to Clipboard")) {
    ImGui::LogToClipboard();
  }

  ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

  ImGuiListClipper clipper;
  clipper.Begin(logs.size());

  while (clipper.Step()) {
    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
      ImGui::TextUnformatted(logs[i].c_str());
    }
  }

  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();
  ImGui::End();
}

void Engine::load_sky(std::string path) { skybox = {path}; }

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
  NFD_Quit();
}
