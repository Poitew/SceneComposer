#include "engine.h"

Assimp::Importer Engine::importer = Assimp::Importer();

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

  // clang-format off
  const aiScene* scene = importer.ReadFile(
    "src/core/spada.glb", 
    aiProcess_CalcTangentSpace | 
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices | 
    aiProcess_SortByPType
  );
  // clang-format on

  if (scene == nullptr) {
    std::cout << importer.GetErrorString() << "\n";
    return false;
  }

  const aiMesh* model = scene->mMeshes[0];

  mesh = {model};

  return true;
}

void Engine::draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  mesh.draw();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool Engine::should_close() { return glfwWindowShouldClose(window); }

Engine::~Engine() { glfwTerminate(); }
