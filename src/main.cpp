#include "core/engine.hpp"
#include "utils/model_loader.hpp"
#include "utils/scene.hpp"

int main(int argc, char* argv[]) {
  bool vr_mode = false;

  if (argc > 1) {
    vr_mode = std::string(argv[1]) == "vr_mode";
  }

  Engine engine{1360, 768, "Composer", vr_mode};

  if (engine.init_application() && engine.init_imgui()) {
    Scene scene;

    Shader& shader = engine.get_shader();
    Shader& picking_shader = engine.get_picking_shader();

    unsigned int selected_id = 0;
    std::string model_path;
    std::string sky_path;
    bool start_rendering = false;

    auto render_world = [&](glm::mat4 v, glm::mat4 p) {
      engine.update_shaders(v, p);

      for (auto& model : scene.get_scene_map()) {
        if (model.second) model.second->draw(shader);
      }

      engine.draw_skybox(v, p);
      engine.draw_icons(v, p);
    };

    while (!engine.should_close()) {
      engine.begin_frame();
      model_path.clear();
      sky_path.clear();
      start_rendering = false;

      if (vr_mode) {
        engine.begin_vr_frame();

        while (engine.next_eye()) {
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          render_world(engine.get_view(), engine.get_proj());
        }

        engine.end_vr_frame();
        render_world(engine.get_view(), engine.get_proj());
      } else {
        engine.begin_picking();
        for (auto& model : scene.get_scene_map()) {
          if (model.second) {
            model.second->draw_picking(picking_shader);
          }
        }
        engine.close_picking();

        render_world(engine.get_view(), engine.get_proj());
      }

      unsigned int model_id = engine.read_click();
      selected_id = model_id > 0 ? model_id : selected_id;

      Model* model = scene.get_model(selected_id);
      if (model) {
        engine.draw_object_properties_panel(model->get_transform(), model->get_hidden_flag());
      }

      engine.draw_main_bar(model_path, sky_path, start_rendering);
      engine.draw_world_properties_panel();
      engine.draw_hierarchy_gui(scene.get_scene_map(), selected_id);
      engine.draw_bottom_log_panel();

      if (start_rendering) {
        engine.begin_render();
        for (auto& model : scene.get_scene_map()) {
          if (model.second) {
            model.second->draw(picking_shader);
          }
        }
        engine.close_render();
      }

      if (!model_path.empty()) {
        scene.add_model(ModelLoader::load(model_path));
      }

      if (!sky_path.empty()) {
        engine.load_sky(sky_path);
      }

      engine.end_frame();
    }
  }

  return 0;
}