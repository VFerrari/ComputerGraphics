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
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test {

class TestTexture2D : public Test {
 private:
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VBO;
  std::unique_ptr<IndexBuffer> m_IBO;
  std::unique_ptr<Shader> m_Shader;
  std::unique_ptr<Texture> m_Texture;

  glm::mat4 m_Proj, m_View;
  glm::vec3 m_TranslationA, m_TranslationB;

  Renderer m_Renderer;

 public:
  TestTexture2D(GUI &gui);

  void OnRender() override;
  void OnGUIRender() override;

 private:
  void DrawTexture(glm::mat4 &model);
};

}  // namespace test