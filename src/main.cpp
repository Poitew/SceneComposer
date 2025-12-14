#include "core/engine.hpp"
#include "renderer/model_loader.hpp"

int main() {
  Engine engine{1920, 780, "Composer"};

  if (engine.init_application()) {
    Mesh sword = ModelLoader::load("src/core/spada.glb");

    while (!engine.should_close()) {
      engine.being_frame();

      sword.draw();

      engine.end_frame();
    }
  }

  return 0;
}