#ifndef MESH_HPP
#define MESH_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "renderer/shader.hpp"
#include "renderer/texture.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normals;
  glm::vec2 texture_coords;
};

class Mesh {
 public:
  Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Texture texture);
  Mesh() = default;
  ~Mesh();

  void draw(Shader& shader, glm::mat4 model, bool is_picking = false);

 private:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  glm::vec2 texture_coords;
  Texture texture;

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

  void setup();
};

#endif