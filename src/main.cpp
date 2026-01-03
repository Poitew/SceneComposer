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

      model = glm::rotate(model, glm::radians(Time::delta() * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      test.draw(shader, model);

      engine.end_frame();
    }
  }

  return 0;
}