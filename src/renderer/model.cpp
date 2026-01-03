#include "model.hpp"

Model::Model(std::vector<std::shared_ptr<Mesh>> model) : model{model} {};

void Model::draw(Shader& shader, glm::mat4 model_mat) {
  for (auto& mesh : model) {
    mesh->draw(shader, model_mat);
  }
}
