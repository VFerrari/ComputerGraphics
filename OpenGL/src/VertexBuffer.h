#pragma once

#include <GL/glew.h>

class VertexBuffer {
 private:
  unsigned int m_RendererID;
  GLenum m_Type;

 public:
  VertexBuffer(const void *data, unsigned int size,
               GLenum usage = GL_STATIC_DRAW);
  ~VertexBuffer();

  void AddData(const void *data, unsigned int size);

  void Bind() const;
  void Unbind() const;
};