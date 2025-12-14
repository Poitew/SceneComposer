#include "model_loader.hpp"

Mesh ModelLoader::load(std::string filepath) {
  Assimp::Importer importer = Assimp::Importer();

  // clang-format off
  const aiScene* scene = importer.ReadFile(
    filepath, 
    aiProcess_CalcTangentSpace | 
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices | 
    aiProcess_SortByPType
  );
  // clang-format on

  if (scene == nullptr) {
    std::cout << importer.GetErrorString() << "\n";
  }

  const aiMesh* model = scene->mMeshes[0];

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

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

  Mesh mesh = {vertices, indices};

  return mesh;
}