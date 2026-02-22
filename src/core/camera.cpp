#include "camera.hpp"

Camera::Camera(float fov, int width, int height, float near_plane, float far_plane)
    : fov{fov}, near_plane{near_plane}, far_plane{far_plane} {
  camPos = glm::vec3(0.0f, 0.0f, 3.0f);
  camFront = glm::vec3(0.0f, 0.0f, -1.0f);
  camUp = glm::vec3(0.0f, 1.0f, 0.0f);

  pitch = 0.0f;
  yaw = -90.0f;
}

void Camera::move(bool forward, bool backward, bool left, bool right, bool up, bool down,
                  bool shift, float delta_time) {
  const float LOW_SPEED = 7.5f;
  const float HIGH_SPEED = 15.0f;
  speed = (shift ? HIGH_SPEED : LOW_SPEED) * delta_time;

  if (forward) camPos += speed * camFront;

  if (backward) camPos -= speed * camFront;

  if (left) camPos -= glm::normalize(glm::cross(camFront, camUp)) * speed;

  if (right) camPos += glm::normalize(glm::cross(camFront, camUp)) * speed;

  if (up) camPos += speed * camUp;

  if (down) camPos -= speed * camUp;
}

void Camera::rotate(float deltaX, float deltaY, float delta_time) {
  const float sensitivity = 0.1f;

  yaw += -deltaX * sensitivity;
  pitch -= -deltaY * sensitivity;

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  camFront = glm::normalize(direction);
}

glm::mat4 Camera::get_base_transform() { return glm::lookAt(camPos, camPos + camFront, camUp); }

glm::mat4 Camera::get_projection(float aspect) {
  return glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
}