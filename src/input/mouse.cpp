#include "mouse.hpp"

bool Mouse::first_mouse = true;

float Mouse::mouse_delta_x = 0.0f;
float Mouse::mouse_delta_y = 0.0f;
float Mouse::last_x = 0.0f;
float Mouse::last_y = 0.0f;

void Mouse::initialize(int width, int height) {
  last_x = width / 2;
  last_y = height / 2;
}

void Mouse::mouse_callback(GLFWwindow* window, double pos_x, double pos_y) {
  if (first_mouse) {
    last_x = pos_x;
    last_y = pos_y;
    first_mouse = false;
  }

  mouse_delta_x = last_x - pos_x;
  mouse_delta_y = last_y - pos_y;

  last_x = pos_x;
  last_y = pos_y;
}

void Mouse::reset_deltas() {
  mouse_delta_x = 0;
  mouse_delta_y = 0;
}

float Mouse::get_delta_x() { return mouse_delta_x; }
float Mouse::get_delta_y() { return mouse_delta_y; }