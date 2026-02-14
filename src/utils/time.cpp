#include "time.hpp"

float EngineTime::last_frame = 0.0f;
float EngineTime::delta_time = 0.0f;

void EngineTime::update(float current_frame) {
  delta_time = current_frame - last_frame;
  last_frame = current_frame;
}

float EngineTime::delta() { return delta_time; }
