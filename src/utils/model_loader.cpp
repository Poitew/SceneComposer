#include "model_loader.hpp"

unsigned int ModelLoader::id_count = 0;

std::shared_ptr<Model> ModelLoader::load(const std::string& filepath) {
  Assimp::Importer importer;

  // clang-format off
  const aiScene* scene = importer.ReadFile(filepath, 
    aiProcess_CalcTangentSpace | 
    aiProcess_Triangulate |                              
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType
  );
  // clang-format on

  if (!scene || !scene->mMeshes[0]) {
    std::cerr << "Error when accessing the scene: " << importer.GetErrorString() << "\n";
  }

  std::vector<std::shared_ptr<Mesh>> meshes;

  for (int i = 0; i < scene->mNumMeshes; i++) {
    meshes.push_back(process_mesh(scene->mMeshes[i], scene));
  }

  id_count++;
  std::string name = scene->mRootNode->mName.C_Str();

  return std::make_shared<Model>(Model{meshes, id_count, name});
}

std::shared_ptr<Mesh> ModelLoader::process_mesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  Texture texture;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex v;

    v.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

    v.normals = mesh->HasNormals()
                    ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                    : glm::vec3(0.0f);

    v.texture_coords = mesh->HasTextureCoords(0)
                           ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                           : glm::vec2(0.0f);

    vertices.push_back(v);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
      indices.push_back(mesh->mFaces[i].mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    aiString path;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
    Texture diffuse_texture{};

    if (path.C_Str()[0] == '*') {
      int index = std::stoi(&path.C_Str()[1]);
      aiTexture* embeddedTex = scene->mTextures[index];
      unsigned char* memory = reinterpret_cast<unsigned char*>(embeddedTex->pcData);

      texture = Texture{memory, embeddedTex->mWidth, "texture_diffuse"};
    } else {
      std::string directory = path.C_Str();
      directory = directory.substr(0, directory.find_last_of("/\\"));

      texture = Texture{path.C_Str(), "texture_diffuse"};
    }
  }

  return std::make_shared<Mesh>(vertices, indices, texture);
}