#pragma once

// STL
#include <vector>

// OpenGL
#include <GL/glew.h>

// Local
#include "Renderer.h"
#include "Vertex.h"

struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int getSizeOfType(unsigned int type) {
    switch (type) {
      case GL_FLOAT:
        return sizeof(float);
      case GL_UNSIGNED_INT:
        return sizeof(unsigned int);
      case GL_UNSIGNED_BYTE:
        return sizeof(unsigned char);
      default:
        ASSERT(false);
        return 0;
    }
  }
};

class VertexBufferLayout {
 private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;

 public:
  VertexBufferLayout() : m_Stride(0) {}

  template <typename T>
  void Push(unsigned int count) {
    ASSERT(false);
  }

  inline const std::vector<VertexBufferElement>& getElements() const {
    return m_Elements;
  }
  inline unsigned int GetStride() const { return m_Stride; }
};

template <>
inline void VertexBufferLayout::Push<float>(unsigned int count) {
  m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
  m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template <>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
  m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
  m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template <>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
  m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
  m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

template <>
inline void VertexBufferLayout::Push<Vertex>(unsigned int count) {
  m_Elements.push_back(
      {GL_FLOAT, sizeof(Vertex::Position) / sizeof(float), GL_FALSE});
  m_Elements.push_back(
      {GL_FLOAT, sizeof(Vertex::Color) / sizeof(float), GL_FALSE});
  m_Elements.push_back(
      {GL_FLOAT, sizeof(Vertex::TexCoords) / sizeof(float), GL_FALSE});
  m_Elements.push_back({GL_FLOAT, 1, GL_FALSE});
  m_Stride += sizeof(Vertex);
}