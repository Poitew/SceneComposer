#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/matrix.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "renderer/mesh.hpp"
#include "renderer/shader.hpp"

struct Transform {
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};

  glm::mat4 get_matrix() const {
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, glm::radians(rotation.x), {1, 0, 0});
    mat = glm::rotate(mat, glm::radians(rotation.y), {0, 1, 0});
    mat = glm::rotate(mat, glm::radians(rotation.z), {0, 0, 1});
    mat = glm::scale(mat, scale);

    return mat;
  }
};

class Model {
 public:
  Model(std::vector<std::shared_ptr<Mesh>> model, unsigned int id, std::string name);
  void draw(Shader& shader);
  void draw_picking(Shader& shader);

  unsigned int get_id();
  std::string& get_name();
  glm::mat4 get_model_matrix();
  Transform& get_transform();
  bool& get_hidden_flag();

 private:
  std::vector<std::shared_ptr<Mesh>> model;
  Transform transform;
  unsigned int id;
  std::string name;
  bool hidden = false;
};

#endif