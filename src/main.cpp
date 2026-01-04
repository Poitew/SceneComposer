#include <glm/matrix.hpp>

#include "core/engine.hpp"
#include "utils/model_loader.hpp"

int main() {
  Engine engine{1360, 768, "Composer"};

  if (engine.init_application() && engine.init_imgui()) {
    Model test = ModelLoader::load("src/core/test.glb");

    Shader& shader = engine.get_shader();

    glm::mat4 model = glm::mat4(1.0f);

    while (!engine.should_close()) {
      engine.begin_frame();
      engine.draw_gui();

      test.draw(shader, model);

      engine.end_frame();
    }
  }

  return 0;
}