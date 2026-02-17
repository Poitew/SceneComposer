#ifndef VR_CONTEXT_HPP
#define VR_CONTEXT_HPP

// clang-format off
#include <glad/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// clang-format on

#define XR_USE_PLATFORM_XLIB
#define XR_USE_GRAPHICS_API_OPENGL
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <cstring>
#include <glm/gtc/quaternion.hpp>
#include <glm/matrix.hpp>
#include <vector>

class VRContext {
 public:
  VRContext() = default;
  VRContext(GLFWwindow* window, int width, int height);
  void begin_frame();
  void end_frame();

  glm::mat4 get_proj();
  glm::mat4 get_view();

 private:
  float near_z;
  float far_z;
  int width;
  int height;
  bool is_running;

  XrInstance instance;
  XrSystemId system;
  XrSession session;
  XrSpace world_space;

  XrFrameState frame_state{XR_TYPE_FRAME_STATE};
  XrSwapchain swapchain;

  std::vector<XrView> eyes;
  std::vector<XrSwapchainImageOpenGLKHR> images;

  XrCompositionLayerProjection projection_layer{XR_TYPE_COMPOSITION_LAYER_PROJECTION};
  std::vector<XrCompositionLayerProjectionView> projection_views;

  glm::mat4 view;
  glm::mat4 proj;

  void poll_events();
};

#endif