#ifndef TIME_HPP
#define TIME_HPP

class Time {
 private:
  static float delta_time;
  static float last_frame;

 public:
  static void update(float current_frame);
  static float delta();
};

#endif