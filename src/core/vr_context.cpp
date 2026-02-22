#include "vr_context.hpp"

VRContext::VRContext(GLFWwindow* window) {
  eyes.resize(2, {XR_TYPE_VIEW});
  projection_views.resize(2, {XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW});

  near_z = 0.1f;
  far_z = 1000.0f;
  current_img = 0;

  glGenFramebuffers(1, &FBO);
  GLuint depth_rb;
  glGenRenderbuffers(1, &depth_rb);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);

  // Instance
  XrApplicationInfo AI;
  strncpy(AI.applicationName, "Composer", XR_MAX_APPLICATION_NAME_SIZE);
  AI.applicationVersion = 1;
  strncpy(AI.engineName, "OpenXR Engine", XR_MAX_ENGINE_NAME_SIZE);
  AI.engineVersion = 1;
  AI.apiVersion = XR_CURRENT_API_VERSION;

  const char* extensions[] = {"XR_KHR_opengl_enable", "GL_EXT_memory_object_fd"};

  XrInstanceCreateInfo instance_info{XR_TYPE_INSTANCE_CREATE_INFO};
  instance_info.applicationInfo = AI;
  instance_info.enabledExtensionCount = 1;
  instance_info.enabledExtensionNames = extensions;

  xrCreateInstance(&instance_info, &instance);

  // Bind to OpenGL
  XrGraphicsBindingOpenGLXlibKHR graphics_binding{XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR};
  graphics_binding.xDisplay = glfwGetX11Display();
  graphics_binding.glxDrawable = glfwGetX11Window(window);
  graphics_binding.glxContext = glfwGetGLXContext(window);

  // System
  XrSystemGetInfo system_info{XR_TYPE_SYSTEM_GET_INFO};
  system_info.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
  xrGetSystem(instance, &system_info, &system);

  // Width - Height
  XrViewConfigurationView view_configs[2] = {{XR_TYPE_VIEW_CONFIGURATION_VIEW},
                                             {XR_TYPE_VIEW_CONFIGURATION_VIEW}};
  uint32_t view_count;

  xrEnumerateViewConfigurationViews(instance, system, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 2,
                                    &view_count, view_configs);

  width = view_configs[0].recommendedImageRectWidth;
  height = view_configs[0].recommendedImageRectHeight;

  // Session
  XrSessionCreateInfo session_info{XR_TYPE_SESSION_CREATE_INFO};
  session_info.next = &graphics_binding;
  session_info.systemId = system;
  PFN_xrGetOpenGLGraphicsRequirementsKHR pfnGetOpenGLGraphicsRequirementsKHR = nullptr;
  xrGetInstanceProcAddr(instance, "xrGetOpenGLGraphicsRequirementsKHR",
                        (PFN_xrVoidFunction*)&pfnGetOpenGLGraphicsRequirementsKHR);

  XrGraphicsRequirementsOpenGLKHR graphicsRequirements{XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR};
  pfnGetOpenGLGraphicsRequirementsKHR(instance, system, &graphicsRequirements);

  xrCreateSession(instance, &session_info, &session);

  // World position
  XrReferenceSpaceCreateInfo space_info{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
  space_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
  space_info.poseInReferenceSpace.orientation.w = 1.0f;

  xrCreateReferenceSpace(session, &space_info, &world_space);

  // Swapchain
  XrSwapchainCreateInfo sc_info{XR_TYPE_SWAPCHAIN_CREATE_INFO};
  sc_info.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
  sc_info.format = GL_SRGB8_ALPHA8;
  sc_info.sampleCount = 1;
  sc_info.width = width;
  sc_info.height = height;
  sc_info.faceCount = 1;
  sc_info.arraySize = 2;
  sc_info.mipCount = 1;

  xrCreateSwapchain(session, &sc_info, &swapchain);

  // Images
  uint32_t count = 0;
  xrEnumerateSwapchainImages(swapchain, 0, &count, nullptr);

  images.resize(count, {XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR});

  xrEnumerateSwapchainImages(swapchain, count, &count, (XrSwapchainImageBaseHeader*)images.data());
}

void VRContext::poll_events() {
  XrEventDataBuffer event{XR_TYPE_EVENT_DATA_BUFFER};

  while (xrPollEvent(instance, &event) == XR_SUCCESS) {
    if (event.type == XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED) {
      auto* state_changed = (XrEventDataSessionStateChanged*)&event;
      XrSessionState current_state = state_changed->state;

      if (current_state == XR_SESSION_STATE_READY) {
        XrSessionBeginInfo begin_info{XR_TYPE_SESSION_BEGIN_INFO};
        begin_info.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

        xrBeginSession(session, &begin_info);
        is_running = true;
      }
      if (current_state == XR_SESSION_STATE_STOPPING) {
        xrEndSession(session);
        is_running = false;
      }
    }
  }
}

void VRContext::begin_frame() {
  poll_events();

  if (!is_running || eyes.size() < 2 || images.empty()) return;

  XrFrameWaitInfo wait_info{XR_TYPE_FRAME_WAIT_INFO};
  xrWaitFrame(session, &wait_info, &frame_state);

  XrFrameBeginInfo begin_info{XR_TYPE_FRAME_BEGIN_INFO};
  xrBeginFrame(session, &begin_info);

  current_eye = -1;
}

bool VRContext::next_eye(glm::mat4& player_body) {
  if (current_eye >= 0) {
    XrSwapchainImageReleaseInfo release_info{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
    xrReleaseSwapchainImage(swapchain, &release_info);
  }

  current_eye++;
  if (current_eye >= 2) return false;

  XrViewLocateInfo locate_info{XR_TYPE_VIEW_LOCATE_INFO, nullptr,
                               XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
                               frame_state.predictedDisplayTime, world_space};
  XrViewState state{XR_TYPE_VIEW_STATE};
  uint32_t count;
  xrLocateViews(session, &locate_info, &state, 2, &count, eyes.data());

  XrSwapchainImageAcquireInfo acq_info{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
  xrAcquireSwapchainImage(swapchain, &acq_info, &current_img);
  XrSwapchainImageWaitInfo wait_info{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, nullptr,
                                     XR_INFINITE_DURATION};
  xrWaitSwapchainImage(swapchain, &wait_info);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, images[current_img].image, 0,
                            current_eye);

  glViewport(0, 0, width, height);

  glm::quat q(eyes[current_eye].pose.orientation.w, eyes[current_eye].pose.orientation.x,
              eyes[current_eye].pose.orientation.y, eyes[current_eye].pose.orientation.z);
  glm::vec3 p(eyes[current_eye].pose.position.x, eyes[current_eye].pose.position.y,
              eyes[current_eye].pose.position.z);

  glm::mat4 head_pose = glm::translate(glm::mat4(1.0f), p) * glm::mat4_cast(q);
  view = glm::inverse(head_pose) * player_body;

  float tL = tanf(eyes[current_eye].fov.angleLeft);
  float tR = tanf(eyes[current_eye].fov.angleRight);
  float tU = tanf(eyes[current_eye].fov.angleUp);
  float tD = tanf(eyes[current_eye].fov.angleDown);
  proj = glm::frustum(tL * near_z, tR * near_z, tD * near_z, tU * near_z, near_z, far_z);

  projection_views[current_eye].pose = eyes[current_eye].pose;
  projection_views[current_eye].fov = eyes[current_eye].fov;
  projection_views[current_eye].subImage.swapchain = swapchain;
  projection_views[current_eye].subImage.imageRect = {{0, 0}, {(int)width, (int)height}};
  projection_views[current_eye].subImage.imageArrayIndex = current_eye;

  return true;
}

void VRContext::end_frame() {
  projection_layer.space = world_space;
  projection_layer.viewCount = 2;
  projection_layer.views = projection_views.data();

  const XrCompositionLayerBaseHeader* submitted_layers[] = {
      (XrCompositionLayerBaseHeader*)&projection_layer};

  XrFrameEndInfo end_info{XR_TYPE_FRAME_END_INFO};
  end_info.displayTime = frame_state.predictedDisplayTime;
  end_info.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
  end_info.layerCount = 1;
  end_info.layers = submitted_layers;

  xrEndFrame(session, &end_info);
}

glm::mat4 VRContext::get_proj() { return proj; }

glm::mat4 VRContext::get_view() { return view; }