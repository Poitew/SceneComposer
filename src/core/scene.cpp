#include "scene.hpp"

Model* Scene::get_model(unsigned int id) {
  if (scene.find(id) == scene.end()) {
    return nullptr;
  }

  return scene[id].get();
}

void Scene::add_model(std::shared_ptr<Model> model) {
  unsigned int id = model->get_id();
  scene[id] = (model);
}

std::unordered_map<unsigned int, std::shared_ptr<Model>>& Scene::get_scene_map() { return scene; }
