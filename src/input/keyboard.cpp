#include "keyboard.hpp"

bool Keyboard::keys[GLFW_KEY_LAST] = {};

void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key < 0) return;

  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;
}

bool Keyboard::is_down(int key) { return keys[key]; }