#include "camera.hpp"

Camera::Camera(float fov, int width, int height, float near_plane, float far_plane) {
  camPos = glm::vec3(0.0f, 0.0f, 3.0f);
  camFront = glm::vec3(0.0f, 0.0f, -1.0f);
  camUp = glm::vec3(0.0f, 1.0f, 0.0f);

  projection =
      glm::perspective(glm::radians(fov), (float)width / (float)height, near_plane, far_plane);

  view = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::move(bool forward, bool backward, bool left, bool right, bool up, bool down,
                  float delta_time) {
  const float speed = 5.0f * delta_time;

  if (forward) camPos += speed * camFront;

  if (backward) camPos -= speed * camFront;

  if (left) camPos -= glm::normalize(glm::cross(camFront, camUp)) * speed;

  if (right) camPos += glm::normalize(glm::cross(camFront, camUp)) * speed;

  if (up) camPos += speed * camUp;

  if (down) camPos -= speed * camUp;

  view = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::rotate(float deltaX, float deltaY, float delta_time) {
  const float sensitivity = 0.1f;

  float yaw_offset = deltaX * sensitivity;
  float pitch_offset = deltaY * sensitivity;

  glm::mat4 yaw_rot = glm::rotate(glm::mat4(1.0f), glm::radians(yaw_offset), camUp);
  camFront = glm::normalize(glm::vec3(yaw_rot * glm::vec4(camFront, 0.0f)));

  glm::vec3 cam_right = glm::normalize(glm::cross(camFront, camUp));

  glm::mat4 pitch_rot = glm::rotate(glm::mat4(1.0f), glm::radians(pitch_offset), cam_right);
  camFront = glm::normalize(glm::vec3(pitch_rot * glm::vec4(camFront, 0.0f)));

  cam_right = glm::normalize(glm::cross(camFront, camUp));
  camUp = glm::normalize(glm::cross(cam_right, camFront));
}

glm::mat4 Camera::get_view() { return view; }

glm::mat4 Camera::get_projection() { return projection; }