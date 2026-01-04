#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
 public:
  Camera(float fov, int width, int height, float near_plane, float far_plane);
  Camera() = default;

  void move(bool forward, bool backward, bool left, bool right, bool up, bool down,
            float delta_time);

  void rotate(float deltaX, float deltaY, float delta_time);
  glm::mat4 get_view();
  glm::mat4 get_projection();

 private:
  glm::vec3 camPos;
  glm::vec3 camFront;
  glm::vec3 camUp;

  glm::mat4 view;
  glm::mat4 projection;

  float yaw;
  float pitch;
};

#endif