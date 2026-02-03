#include "icon_model.hpp"

IconModel::IconModel(std::string icon_path) {
  float size = 0.1f;
  GLfloat vertices[] = {-size, -size, 0.0f, 0.0f, 0.0f, size, -size, 0.0f, 1.0f, 0.0f,
                        -size, size,  0.0f, 0.0f, 1.0f, size, size,  0.0f, 1.0f, 1.0f};

  GLuint indices[] = {0, 1, 2, 2, 1, 3};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  icon_shader = {"shaders/icon.vs", "shaders/icon.fs"};
  icon_texture = {icon_path, "texture_diffuse"};
}

void IconModel::draw(glm::mat4& view, glm::mat4& projection) {
  icon_shader.use();

  model[0][0] = view[0][0];
  model[0][1] = view[1][0];
  model[0][2] = view[2][0];
  model[1][0] = view[0][1];
  model[1][1] = view[1][1];
  model[1][2] = view[2][1];
  model[2][0] = view[0][2];
  model[2][1] = view[1][2];
  model[2][2] = view[2][2];

  icon_shader.set_mat4("model", model);
  icon_shader.set_mat4("view", view);
  icon_shader.set_mat4("projection", projection);

  icon_texture.use(GL_TEXTURE0);

  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glDisable(GL_BLEND);
}

void IconModel::set_model_matrix(glm::mat4& matrix) { model = matrix; }