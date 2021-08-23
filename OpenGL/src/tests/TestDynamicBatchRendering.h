#pragma once

// Parent
#include "Test.h"

// STL
#include <memory>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Local
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test {

class TestDynamicBatchRendering : public Test {
 private:
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VBO;
  std::unique_ptr<IndexBuffer> m_IBO;
  std::unique_ptr<Shader> m_Shader;
  std::unique_ptr<Texture> m_TextureA, m_TextureB;

  glm::mat4 m_Proj, m_View;

  Renderer m_Renderer;

  // GUI
  float m_QuadPosition[2] = {-1.5f, -0.5f};
  glm::vec3 m_Translation;

 public:
  TestDynamicBatchRendering(GUI &gui);

  void OnRender() override;
  void OnGUIRender() override;

 private:
  Vertex *CreateQuad(Vertex *target, float x, float y, float textureID);
};

}  // namespace test