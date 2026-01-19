#include "model.hpp"

Model::Model(std::vector<std::shared_ptr<Mesh>> model, unsigned int id, std::string name)
    : model{model}, id{id}, name{name} {};

void Model::draw(Shader& shader) {
  shader.use();

  for (auto& mesh : model) {
    mesh->draw(shader, transform.get_matrix());
  }
}

void Model::draw_picking(Shader& picking_shader) {
  picking_shader.use();
  picking_shader.set_float("modelID", id);

  for (auto& mesh : model) {
    mesh->draw(picking_shader, transform.get_matrix(), true);
  }
}

unsigned int Model::get_id() { return id; }

std::string& Model::get_name() { return name; }

glm::mat4 Model::get_model_matrix() { return transform.get_matrix(); }

Transform& Model::get_transform() { return transform; }
