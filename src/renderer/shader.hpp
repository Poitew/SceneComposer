#ifndef SHADER_HPP
#define SHADER_HPP

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>

class Shader {
 public:
  Shader(std::string vertex_shader, std::string fragment_shader);
  Shader() = default;
  void use();
  void set_mat4(std::string name, glm::mat4 val);
  void set_int(std::string name, int val);
  void set_float(std::string name, float val);

 private:
  GLuint vertex_shader;
  GLuint fragment_shader;
  GLuint shader_program;

  void compile_shader(std::string& shader_path, GLuint& shader);
  std::string load_shader(std::string filename);
};

#endif