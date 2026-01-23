#include <glm/matrix.hpp>
#include <memory>

#include "core/engine.hpp"
#include "core/scene.hpp"
#include "utils/model_loader.hpp"

int main() {
  Engine engine{1360, 768, "Composer"};

  if (engine.init_application() && engine.init_imgui()) {
    Scene scene;
    scene.add_model(ModelLoader::load("/home/poitew/blender/pyro.glb"));
    scene.add_model(ModelLoader::load("/home/poitew/blender/cartoon_house/house.fbx"));
    scene.add_model(ModelLoader::load("/home/poitew/blender/test.glb"));

    Shader& shader = engine.get_shader();
    Shader& picking_shader = engine.get_picking_shader();

    unsigned int selected_id = 0;
    std::string model_path;
    std::string sky_path;

    while (!engine.should_close()) {
      engine.begin_frame();
      model_path.clear();
      sky_path.clear();

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

      engine.draw_skybox();

      unsigned int model_id = engine.read_click();
      if (model_id > 0) {
        selected_id = model_id;
      }

      Model* model = scene.get_model(selected_id);
      if (model) {
        engine.draw_picker_gui(model->get_transform());
      }

      engine.draw_main_bar_gui(model_path, sky_path);
      engine.draw_hierarchy_gui(scene.get_scene_map(), selected_id);

      if (!model_path.empty() || model_path != "") {
        scene.add_model(ModelLoader::load(model_path));
      }

      if (!sky_path.empty() || sky_path != "") {
        engine.load_sky(sky_path);
      }

      engine.end_frame();
    }
  }

  return 0;
}