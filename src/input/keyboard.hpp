#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <GLFW/glfw3.h>

class Keyboard {
 public:
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static bool is_down(int key);

 private:
  static bool keys[GLFW_KEY_LAST];
};

#endif