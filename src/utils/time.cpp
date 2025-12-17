#include "time.hpp"

float Time::last_frame = 0.0f;
float Time::delta_time = 0.0f;

void Time::update(float current_frame) {
  delta_time = current_frame - last_frame;
  last_frame = current_frame;
}

float Time::delta() { return delta_time; }
