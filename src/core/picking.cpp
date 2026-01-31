#include "picking.hpp"

bool Picking::init(int width, int height) {
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  glGenTextures(1, &m_picking_texture);
  glBindTexture(GL_TEXTURE_2D, m_picking_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_picking_texture, 0);

  glGenTextures(1, &m_depth_texture);
  glBindTexture(GL_TEXTURE_2D, m_depth_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
               NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

  glReadBuffer(GL_NONE);

  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::string entry = "Status: " + std::to_string(status);
    Logger::log(entry);
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

void Picking::enable_writing() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo); }
void Picking::disable_writing() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }

float Picking::read_pixel(unsigned int x, unsigned int y) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
  glReadBuffer(GL_COLOR_ATTACHMENT0);

  float pixel;
  glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, &pixel);

  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

  return pixel;
}