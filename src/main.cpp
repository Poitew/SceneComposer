#include <glm/matrix.hpp>

#include "core/engine.hpp"
#include "renderer/model_loader.hpp"

int main() {
  Engine engine{1920, 780, "Composer"};

  if (engine.init_application()) {
    Mesh sword = ModelLoader::load("src/core/spada.glb");
    Shader& shader = engine.get_shader();

    glm::mat4 model = glm::mat4(1.0f);

    while (!engine.should_close()) {
      engine.begin_frame();

      model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      sword.draw(shader, model);

      engine.end_frame();
    }
  }

  return 0;
}