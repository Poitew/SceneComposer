#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "renderer/mesh.hpp"
#include "renderer/model.hpp"
#include "renderer/texture.hpp"

class ModelLoader {
 public:
  ModelLoader() = delete;
  static Model load(const std::string& filepath);

 private:
  static std::shared_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene);
  static unsigned int id_count;
};

#endif