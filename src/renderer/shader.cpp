#include "shader.hpp"

std::string Shader::load_shader(std::string filename) {
  std::ifstream file;
  std::stringstream buffer;

  std::string ret{};

  file.open(filename);

  if (file.is_open()) {
    buffer << file.rdbuf();
    ret = buffer.str();
  } else {
    std::cout << "Could not open shader\n";
  }

  file.close();

  return ret;
}

void Shader::compile_shader(std::string& shader_path, GLuint& shader) {
  int success;
  char log[512];

  std::string shader_src = load_shader(shader_path);
  const char* shader_src_c = shader_src.c_str();

  glShaderSource(shader, 1, &shader_src_c, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, log);
    std::cout << "Error in the shader:\n" << log << "\n";
  }
}

Shader::Shader(std::string vertex_path, std::string fragment_path) {
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  compile_shader(vertex_path, vertex_shader);
  compile_shader(fragment_path, fragment_shader);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::set_mat4(std::string name, glm::mat4 val) {
  GLuint trans_location = glGetUniformLocation(shader_program, name.c_str());
  glUniformMatrix4fv(trans_location, 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::set_vec4(std::string name, glm::vec4 val) {
  GLuint vec_location = glGetUniformLocation(shader_program, name.c_str());
  glUniform4fv(vec_location, 1, glm::value_ptr(val));
}

void Shader::set_int(std::string name, int val) {
  GLuint int_location = glGetUniformLocation(shader_program, name.c_str());
  glUniform1i(int_location, val);
}

void Shader::set_float(std::string name, float val) {
  GLuint float_location = glGetUniformLocation(shader_program, name.c_str());
  glUniform1f(float_location, val);
}

void Shader::use() { glUseProgram(shader_program); }