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
    std::string error{importer.GetErrorString()};
    Logger::log("Error when accessing the scene: " + error);
  }

  std::vector<std::shared_ptr<Mesh>> meshes;

  for (int i = 0; i < scene->mNumMeshes; i++) {
    meshes.push_back(process_mesh(scene->mMeshes[i], scene, filepath));
  }

  id_count++;
  std::string name = scene->mRootNode->mName.C_Str();

  return std::make_shared<Model>(Model{meshes, id_count, name});
}

std::shared_ptr<Mesh> ModelLoader::process_mesh(aiMesh* mesh, const aiScene* scene,
                                                const std::string& filepath) {
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

  glm::vec4 material_color;

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    aiString path;
    aiColor4D color(1.0f, 1.0f, 1.0f, 1.0f);

    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
      Texture diffuse_texture{};

      if (path.C_Str()[0] == '*') {
        int index = std::stoi(&path.C_Str()[1]);
        aiTexture* embeddedTex = scene->mTextures[index];
        unsigned char* memory = reinterpret_cast<unsigned char*>(embeddedTex->pcData);

        texture = Texture{memory, embeddedTex->mWidth, "texture_diffuse"};
      } else {
        std::string model_dir = filepath.substr(0, filepath.find_last_of("/\\"));
        std::string full_path = model_dir + "/" + path.C_Str();

        texture = Texture{full_path.c_str(), "texture_diffuse"};
      }
    }

    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) != AI_SUCCESS) {
      material->Get(AI_MATKEY_BASE_COLOR, color);
    }

    material_color = glm::vec4(color.r, color.g, color.b, color.a);
  }

  return std::make_shared<Mesh>(vertices, indices, texture, material_color);
}