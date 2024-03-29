#include "Shader.h"

// STL
#include <fstream>
#include <iostream>
#include <sstream>

// Local
#include "Renderer.h"

Shader::Shader(const std::string &filepath)
    : m_FilePath(filepath), m_RendererID(0) {
  ShaderProgramSource source = ParseShader(filepath);
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

ShaderProgramSource Shader::ParseShader(const std::string &filepath) {
  std::ifstream stream(filepath);

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << "\n";
    }
  }

  return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string &source) {
  GLCall(unsigned int id = glCreateShader(type));
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char *message = (char *)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cerr << "Failed to compile"
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!"
              << std::endl;
    std::cerr << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader,
                                  const std::string &fragmentShader) {
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

void Shader::Bind() const { GLCall(glUseProgram(m_RendererID)); }

void Shader::Unbind() const { GLCall(glUseProgram(0)); }

void Shader::SetUniform1i(const std::string &name, int value) {
  GLint location = GetUniformLocation(name);
  GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string &name, float value) {
  GLint location = GetUniformLocation(name);
  GLCall(glUniform1f(location, value));
}

void Shader::SetUniform4f(const std::string &name, const glm::vec4 &values) {
  GLint location = GetUniformLocation(name);
  GLCall(glUniform4f(location, values.x, values.y, values.z, values.w));
}

void Shader::SetUniform1iv(const std::string &name, int count, int *value) {
  GLint location = GetUniformLocation(name);
  GLCall(glUniform1iv(location, count, value));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
  GLint location = GetUniformLocation(name);
  GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

GLint Shader::GetUniformLocation(const std::string &name) const {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1)
    std::cout << "Warning: uniform '" << name << "' doesn't exist!"
              << std::endl;

  m_UniformLocationCache[name] = location;
  return location;
}