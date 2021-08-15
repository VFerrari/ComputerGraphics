#pragma once

// Parent
#include "Test.h"

// STL
#include <memory>

// Local
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test {

class TestBlinkingSquare : public Test {
 private:
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VBO;
  std::unique_ptr<IndexBuffer> m_IBO;
  std::unique_ptr<Shader> m_Shader;

  float m_Red, m_Increment;

  Renderer m_Renderer;

 public:
  TestBlinkingSquare(GUI &gui);

  void OnRender() override;
  void OnGUIRender() override;

 private:
  void IncrementColor();
};

}  // namespace test