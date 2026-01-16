#include "skybox.hpp"

std::vector<GLfloat> Skybox::skybox_vertices = {
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

void Skybox::create_geometry() {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, skybox_vertices.size() * sizeof(float), skybox_vertices.data(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Skybox::create_cubemap() {
  const int RES = 1024;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, RES, RES, 0, GL_RGB, GL_FLOAT,
                 nullptr);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Skybox::load_and_convert_hdri(std::string& hdr_path) {
  GLuint hdr2D;

  int width, height, n_channels;
  stbi_set_flip_vertically_on_load(true);
  float* data = stbi_loadf(hdr_path.c_str(), &width, &height, &n_channels, 0);

  if (data) {
    glGenTextures(1, &hdr2D);
    glBindTexture(GL_TEXTURE_2D, hdr2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    convert_hdri(hdr2D);
    stbi_image_free(data);
  } else {
    std::cout << "Cubemap texture failed to load at path: " << hdr_path << std::endl;
  }
}

void Skybox::convert_hdri(GLuint hdr) {
  const int RES = 1024;

  using namespace glm;

  mat4 capture_projection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
  mat4 capture_views[] = {
      lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
      lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
      lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
      lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
      lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
      lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))};

  conversion_shader = {"shaders/to_cubemap.vs", "shaders/to_cubemap.fs"};
  conversion_shader.use();
  conversion_shader.set_int("equirectangularMap", 0);
  conversion_shader.set_mat4("projection", capture_projection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, hdr);

  glGenFramebuffers(1, &FBO);
  glGenRenderbuffers(1, &RBO);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, RES, RES);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

  glViewport(0, 0, RES, RES);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  for (unsigned int i = 0; i < 6; i++) {
    conversion_shader.set_mat4("view", capture_views[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                           textureID, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDeleteTextures(1, &hdr);
  glDeleteFramebuffers(1, &FBO);
  glDeleteRenderbuffers(1, &RBO);
}

Skybox::Skybox(std::string hdr_path) {
  create_geometry();
  create_cubemap();
  load_and_convert_hdri(hdr_path);

  skybox_shader = {"shaders/skybox.vs", "shaders/skybox.fs"};
  skybox_shader.use();
  skybox_shader.set_int("skybox", 0);
}

void Skybox::draw(glm::mat4 projection, glm::mat4 view) {
  glDepthFunc(GL_LEQUAL);

  skybox_shader.use();

  glm::mat4 static_view = glm::mat4(glm::mat3(view));
  skybox_shader.set_mat4("view", static_view);
  skybox_shader.set_mat4("projection", projection);

  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDepthFunc(GL_LESS);
}
