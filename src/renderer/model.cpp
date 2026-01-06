#include "model.hpp"

Model::Model(std::vector<std::shared_ptr<Mesh>> model, unsigned int id) : model{model}, id{id} {};

void Model::draw(Shader& shader, glm::mat4 model_mat) {
  shader.use();

  for (auto& mesh : model) {
    mesh->draw(shader, model_mat);
  }
}

void Model::draw_picking(Shader& picking_shader, glm::mat4 model_mat) {
  picking_shader.use();
  picking_shader.set_float("modelID", id);

  for (auto& mesh : model) {
    mesh->draw(picking_shader, model_mat, true);
  }
}
