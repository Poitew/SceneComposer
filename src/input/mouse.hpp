#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <GLFW/glfw3.h>

class Mouse {
 private:
  static float last_x;
  static float last_y;
  static bool first_mouse;

  static float mouse_delta_x;
  static float mouse_delta_y;

 public:
  static void initialize(int width, int height);
  static void mouse_callback(GLFWwindow* window, double pos_x, double pos_y);

  static void reset_deltas();
  static float get_delta_x();
  static float get_delta_y();
};

#endif