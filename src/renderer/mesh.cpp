#include "mesh.h"

void Mesh::get_data(const aiMesh* model) {
  for (unsigned int i = 0; i < model->mNumVertices; i++) {
    Vertex v;

    v.position = {model->mVertices[i].x, model->mVertices[i].y, model->mVertices[i].z};

    if (model->HasNormals()) {
      v.normals = {model->mNormals[i].x, model->mNormals[i].y, model->mNormals[i].z};
    } else {
      v.normals = glm::vec3(0.0f);
    }

    if (model->HasTextureCoords(0)) {
      v.texture_coords = {model->mTextureCoords[0][i].x, model->mTextureCoords[0][i].y};
    } else {
      v.texture_coords = glm::vec2(0.0f);
    }

    vertices.push_back(v);
  }

  for (unsigned int i = 0; i < model->mNumFaces; i++) {
    const aiFace& face = model->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }
}

Mesh::Mesh(const aiMesh* model) {
  get_data(model);
  setup();
}

void Mesh::setup() {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0],
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, texture_coords));
}

void Mesh::draw() {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}