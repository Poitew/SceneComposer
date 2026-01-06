#include <glm/matrix.hpp>

#include "core/engine.hpp"
#include "utils/model_loader.hpp"

int main() {
  Engine engine{1360, 768, "Composer"};

  if (engine.init_application() && engine.init_imgui()) {
    Model test = ModelLoader::load("src/core/test.glb");
    Model test_2 = ModelLoader::load("src/core/test.glb");
    Shader& shader = engine.get_shader();
    Shader& picking_shader = engine.get_picking_shader();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model_2 = glm::mat4(1.0f);
    model_2 = glm::translate(model_2, glm::vec3(5.0f, 0.0f, 0.0f));

    while (!engine.should_close()) {
      engine.begin_frame();

      engine.begin_picking();
      test.draw_picking(picking_shader, model);
      test_2.draw_picking(picking_shader, model_2);
      engine.close_picking();

      test.draw(shader, model);
      test_2.draw(shader, model_2);

      engine.draw_gui();
      engine.read_click();
      engine.end_frame();
    }
  }

  return 0;
}