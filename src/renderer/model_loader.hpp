#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <iostream>
#include <string>

#include "mesh.hpp"

class ModelLoader {
 public:
  ModelLoader() = delete;
  static Mesh load(std::string filepath);
};

#endif