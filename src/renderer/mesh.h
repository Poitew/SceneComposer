#ifndef MESH_H
#define MESH_H

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normals;
  glm::vec2 texture_coords;
};

class Mesh {
 public:
  Mesh(const aiMesh* model);
  Mesh() = default;

  void get_data(const aiMesh* model);
  void draw();

 private:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  glm::vec2 texture_coords;

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

  void setup();
};

#endif