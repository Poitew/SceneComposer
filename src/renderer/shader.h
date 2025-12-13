#ifndef SHADER_H
#define SHADER_H

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>

class Shader {
 public:
  Shader(std::string vertex_shader, std::string fragment_shader);
  Shader() = default;
  void use();

 private:
  GLuint vertex_shader;
  GLuint fragment_shader;
  GLuint shader_program;

  void compile_shader(std::string& shader_path, GLuint& shader);
  std::string load_shader(std::string filename);
};

#endif