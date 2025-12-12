#include "core/engine.h"

int main() {
  Engine engine{1920, 780, "Composer"};

  if (engine.init_application()) {
    while (!engine.should_close()) {
      engine.draw();
    }
  }

  return 0;
}