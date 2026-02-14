#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "renderer/model.hpp"

using CScene = std::unordered_map<unsigned int, std::shared_ptr<Model>>;

class Scene {
 private:
  CScene scene;

 public:
  CScene& get_scene_map();
  Model* get_model(unsigned int id);
  void add_model(std::shared_ptr<Model> model);
};

#endif