#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "renderer.hpp"

#include <stb/stb_image_write.h>

Renderer::Renderer(int width, int height) : width{width}, height{height} {
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    Logger::log("The FrameBuffer Object is not complete");
  }

  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    Logger::log("The RenderBuffer Object is not complete");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::enable_writing() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
  glViewport(0, 0, width, height);
}

void Renderer::disable_writing() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }

void Renderer::save_to_file() {
  unsigned char* pixels = new unsigned char[width * height * 3];
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  stbi_flip_vertically_on_write(true);
  stbi_write_png("render.png", width, height, 3, pixels, width * 3);
  delete[] pixels;
}
