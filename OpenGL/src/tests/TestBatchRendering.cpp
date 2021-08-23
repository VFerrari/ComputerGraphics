#include "TestBatchRendering.h"

// Local
#include "VertexBufferLayout.h"

// Constants
#include "Settings.h"

namespace test {
TestBatchRendering::TestBatchRendering(GUI &gui)
    : m_Proj(glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0f, 1.0f, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      Test(gui) {
  GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));

  /**
   * Vertices
   *  Layout:
   *    - 2 positions (x,y)
   *    - 4 colors (rgba)
   *    - 2 texture positions
   *    - 1 texture ID
   */
  float positions[] = {
      -1.5f, -0.5f, 0.18f, 0.6f,  0.96f, 1.0f, 0.0f, 0.0f, 0.0f,  // 0
      -0.5f, -0.5f, 0.18f, 0.6f,  0.96f, 1.0f, 1.0f, 0.0f, 0.0f,  // 1
      -0.5f, 0.5f,  0.18f, 0.6f,  0.96f, 1.0f, 1.0f, 1.0f, 0.0f,  // 2
      -1.5f, 0.5f,  0.18f, 0.6f,  0.96f, 1.0f, 0.0f, 1.0f, 0.0f,  // 3

      0.5f,  -0.5f, 1.0f,  0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,  // 0
      1.5f,  -0.5f, 1.0f,  0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,  // 1
      1.5f,  0.5f,  1.0f,  0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,  // 2
      0.5f,  0.5f,  1.0f,  0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f   // 3
  };

  unsigned int indices[] = {
      0, 1, 2, 2, 3, 0,  // 1st quad
      4, 5, 6, 6, 7, 4   // 2nd quad
  };

  /* Blending */
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  /* Vertex Array and Buffer */
  m_VAO = std::make_unique<VertexArray>();
  m_VBO = std::make_unique<VertexBuffer>(positions, sizeof(positions));

  /* Vertex Attributes */
  VertexBufferLayout layout;
  layout.Push<float>(2);  // Positions
  layout.Push<float>(4);  // Colors
  layout.Push<float>(2);  // Texture pos.
  layout.Push<float>(1);  // Texture ID
  m_VAO->AddBuffer(*m_VBO, layout);

  /* Index Buffer */
  m_IBO = std::make_unique<IndexBuffer>(indices, 12);

  /* Shader */
  m_Shader = std::make_unique<Shader>("../res/shaders/Batch.shader");
  m_Shader->Bind();

  int samplers[2] = {0, 1};
  m_Shader->SetUniform1iv("u_Textures", 2, samplers);

  /* Texture */
  m_TextureA = std::make_unique<Texture>("../res/textures/ChernoLogo.png");
  m_TextureB = std::make_unique<Texture>("../res/textures/Expedition.png");
}

void TestBatchRendering::OnRender() {
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  glm::mat4 MVP = m_Proj * m_View * m_Model;
  m_Shader->Bind();

  // Set Textures
  m_TextureA->Bind(0);
  m_TextureB->Bind(1);

  m_Shader->SetUniformMat4f("u_MVP", MVP);
  m_Renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
}

void TestBatchRendering::OnGUIRender() { GetGUI().ShowFramerate(); }

}  // namespace test