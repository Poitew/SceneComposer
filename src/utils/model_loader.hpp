#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>

#include "renderer/mesh.hpp"
#include "renderer/model.hpp"
#include "renderer/texture.hpp"
#include "utils/logger.hpp"

class ModelLoader {
 public:
  ModelLoader() = delete;
  static std::shared_ptr<Model> load(const std::string& filepath);

 private:
  static std::shared_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene,
                                            const std::string& filepath);
  static unsigned int id_count;
};

#endif