#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/matrix.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "renderer/mesh.hpp"
#include "renderer/shader.hpp"

class Model {
 public:
  Model(std::vector<std::shared_ptr<Mesh>> model);
  void draw(Shader& shader, glm::mat4 model_mat);

 private:
  std::vector<std::shared_ptr<Mesh>> model;
};

#endif