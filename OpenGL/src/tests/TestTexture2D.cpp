#include "TestTexture2D.h"

// Local
#include "VertexBufferLayout.h"

// Constants
#include "Settings.h"

namespace test {
TestTexture2D::TestTexture2D(GUI &gui)
    : m_Proj(glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      m_TranslationA(200, 200, 0),
      m_TranslationB(400, 200, 0),
      Test(gui) {
  float positions[] = {
      -50.0f, -50.0f, 0.0f, 0.0f,  // 0
      50.0f,  -50.0f, 1.0f, 0.0f,  // 1
      50.0f,  50.0f,  1.0f, 1.0f,  // 2
      -50.0f, 50.0f,  0.0f, 1.0f   // 3
  };

  unsigned int indices[] = {
      0, 1, 2,  // 1st triangle
      2, 3, 0   // 2nd triangle
  };

  /* Blending */
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  /* Vertex Array and Buffer */
  m_VAO = std::make_unique<VertexArray>();
  m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

  /* Vertex Attributes */
  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  m_VAO->AddBuffer(*m_VBO, layout);

  /* Index Buffer */
  m_IBO = std::make_unique<IndexBuffer>(indices, 6);

  /* Shader */
  m_Shader = std::make_unique<Shader>("../res/shaders/Texture.shader");
  m_Shader->Bind();

  /* Get Texture */
  m_Texture = std::make_unique<Texture>("../res/textures/ChernoLogo.png");
  m_Shader->SetUniform1i("u_Texture", 0);
}

void TestTexture2D::OnRender() {
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  glm::mat4 model;
  m_Texture->Bind();

  /* Object A */
  model = glm::translate(glm::mat4(1.0f), m_TranslationA);
  DrawTexture(model);

  /* Object B */
  model = glm::translate(glm::mat4(1.0f), m_TranslationB);
  DrawTexture(model);
}

void TestTexture2D::OnGUIRender() {
  GUI &gui = GetGUI();
  gui.ShowSlider3f("Translation A", &m_TranslationA.x, 0.0f, WIDTH);
  gui.ShowSlider3f("Translation B", &m_TranslationB.x, 0.0f, WIDTH);
  gui.ShowFramerate();
}

void TestTexture2D::DrawTexture(glm::mat4 &model) {
  glm::mat4 MVP;
  MVP = m_Proj * m_View * model;
  m_Shader->Bind();
  m_Shader->SetUniformMat4f("u_MVP", MVP);
  m_Renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
}

}  // namespace test