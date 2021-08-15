#include "TestBlinkingSquare.h"

// Local
#include "VertexBufferLayout.h"

namespace test {
TestBlinkingSquare::TestBlinkingSquare(GUI &gui)
    : m_Red(0.0f), m_Increment(0.05f), Test(gui) {
  float positions[] = {
      -0.5f, -0.5f,  // 0
      0.5f,  -0.5f,  // 1
      0.5f,  0.5f,   // 2
      -0.5f, 0.5f    // 3
  };

  unsigned int indices[] = {
      0, 1, 2,  // 1st triangle
      2, 3, 0   // 2nd triangle
  };

  /* Vertex Array and Buffer */
  m_VAO = std::make_unique<VertexArray>();
  m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));

  /* Vertex Attributes */
  VertexBufferLayout layout;
  layout.Push<float>(2);
  m_VAO->AddBuffer(*m_VBO, layout);

  /* Index Buffer */
  m_IBO = std::make_unique<IndexBuffer>(indices, 6);

  /* Shader */
  m_Shader = std::make_unique<Shader>("../res/shaders/Basic.shader");
}

void TestBlinkingSquare::OnRender() {
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  m_Renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

  /* Set color and apply change. */
  m_Shader->Bind();
  m_Shader->SetUniform4f("u_Color", m_Red, 0.3f, 0.8f, 1.0f);
  IncrementColor();
}

void TestBlinkingSquare::OnGUIRender() { GetGUI().ShowFramerate(); }

void TestBlinkingSquare::IncrementColor() {
  if (m_Red > 1.0f)
    m_Increment = -0.05f;
  else if (m_Red < 0.0f)
    m_Increment = 0.05f;

  m_Red += m_Increment;
}

}  // namespace test