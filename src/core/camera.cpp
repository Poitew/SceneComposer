#include "camera.hpp"

Camera::Camera(float fov, int width, int height, float near_plane, float far_plane) {
  camPos = glm::vec3(0.0f, 0.0f, 3.0f);
  camFront = glm::vec3(0.0f, 0.0f, -1.0f);
  camUp = glm::vec3(0.0f, 1.0f, 0.0f);

  projection = (glm::radians(fov), (float)width / height, near_plane, far_plane);

  view = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::move(bool forward, bool backward, bool left, bool right, bool up, bool down,
                  float delta_time) {
  const float speed = 1.0f * delta_time;

  if (forward) camPos += speed * camFront;

  if (backward) camPos -= speed * camFront;

  if (left) camPos -= glm::normalize(glm::cross(camFront, camUp)) * speed;

  if (right) camPos += glm::normalize(glm::cross(camFront, camUp)) * speed;

  if (up) camPos += speed * camUp;

  if (down) camPos -= speed * camUp;

  view = glm::lookAt(camPos, camPos + camFront, camUp);
}

/* TO-DO */
void Camera::rotate(float deltaX, float deltaY, float delta_time) {}

glm::mat4 Camera::get_view() { return view; }

glm::mat4 Camera::get_projection() { return projection; }