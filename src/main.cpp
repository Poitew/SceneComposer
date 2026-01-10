#include <glm/matrix.hpp>
#include <memory>

#include "core/engine.hpp"
#include "core/scene.hpp"
#include "utils/model_loader.hpp"

int main() {
  Engine engine{1360, 768, "Composer"};

  if (engine.init_application() && engine.init_imgui()) {
    Scene scene;
    scene.add_model(ModelLoader::load("src/core/test.glb"));
    scene.add_model(ModelLoader::load("src/core/pyro.glb"));

    Shader& shader = engine.get_shader();
    Shader& picking_shader = engine.get_picking_shader();

    unsigned int selected_id = 0;

    while (!engine.should_close()) {
      engine.begin_frame();

      engine.begin_picking();
      for (auto& model : scene.get_scene_map()) {
        if (model.second) {
          model.second->draw_picking(picking_shader);
        }
      }
      engine.close_picking();

      for (auto& model : scene.get_scene_map()) {
        if (model.second) {
          model.second->draw(shader);
        }
      }

      unsigned int model_id = engine.read_click();

      if (model_id > 0) {
        selected_id = model_id;
      }

      Model* model = scene.get_model(selected_id);

      if (model) {
        engine.draw_picker_gui(model->get_transform());
      }
      engine.end_frame();
    }
  }

  return 0;
}