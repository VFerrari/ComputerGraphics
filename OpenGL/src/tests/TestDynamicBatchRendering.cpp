#include "TestDynamicBatchRendering.h"

// Local
#include "VertexBufferLayout.h"

// Constants
#include "Settings.h"

namespace test {
TestDynamicBatchRendering::TestDynamicBatchRendering(GUI &gui)
    : m_Proj(glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0f, 1.0f, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      Test(gui) {
  GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));

  /* Constants for sizes */
  const size_t MaxQuadCount = 1000;
  const size_t MaxVertexCount = MaxQuadCount * 4;
  const size_t MaxIndexCount = MaxQuadCount * 6;

  /* Blending */
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  /* Vertex Array and Buffer */
  m_VAO = std::make_unique<VertexArray>();
  m_VBO = std::make_unique<VertexBuffer>(
      nullptr, sizeof(Vertex) * MaxVertexCount, GL_DYNAMIC_DRAW);

  /* Vertex Attributes */
  VertexBufferLayout layout;
  layout.Push<Vertex>(1);
  m_VAO->AddBuffer(*m_VBO, layout);

  /* Index Buffer */
  uint32_t indices[MaxIndexCount];
  uint32_t offset = 0;
  for (size_t i = 0; i < MaxIndexCount; i += 6) {
    indices[i + 0] = 0 + offset;
    indices[i + 1] = 1 + offset;
    indices[i + 2] = 2 + offset;

    indices[i + 3] = 2 + offset;
    indices[i + 4] = 3 + offset;
    indices[i + 5] = 0 + offset;

    offset += 4;
  }
  m_IBO = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

  /* Shader */
  m_Shader = std::make_unique<Shader>("../res/shaders/Batch.shader");
  m_Shader->Bind();

  int samplers[2] = {0, 1};
  m_Shader->SetUniform1iv("u_Textures", 2, samplers);

  /* Texture */
  m_TextureA = std::make_unique<Texture>("../res/textures/ChernoLogo.png");
  m_TextureB = std::make_unique<Texture>("../res/textures/Expedition.png");
}

void TestDynamicBatchRendering::OnRender() {
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
  m_Shader->Bind();

  // Set Textures
  m_TextureA->Bind(0);
  m_TextureB->Bind(1);

  // Set MVP
  glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
  glm::mat4 MVP = m_Proj * m_View * model;
  m_Shader->SetUniformMat4f("u_MVP", MVP);

  // Set dynamic vertex buffer.
  std::array<Vertex, 1000> vertices;
  Vertex *buffer = vertices.data();
  unsigned int IndexCount = 0;
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) {
      buffer = CreateQuad(buffer, x, y, (x + y) % 2);
      IndexCount += 6;
    }
  }
  buffer = CreateQuad(buffer, m_QuadPosition[0], m_QuadPosition[1], 0.0f);
  IndexCount += 6;

  m_VBO->AddData(vertices.data(), vertices.size() * sizeof(Vertex));
  m_IBO->SetCount(IndexCount);

  // Draw
  m_VAO->Bind();
  m_Renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
}

Vertex *TestDynamicBatchRendering::CreateQuad(Vertex *target, float x, float y,
                                              float textureID) {
  float size = 1.0f;

  target->Position = {x, y};
  target->Color = {0.18f, 0.6f, 0.96f, 1.0f};
  target->TexCoords = {0.0f, 0.0f};
  target->TexID = textureID;
  target++;

  target->Position = {x + size, y};
  target->Color = {0.18f, 0.6f, 0.96f, 1.0f};
  target->TexCoords = {1.0f, 0.0f};
  target->TexID = textureID;
  target++;

  target->Position = {x + size, y + size};
  target->Color = {0.18f, 0.6f, 0.96f, 1.0f};
  target->TexCoords = {1.0f, 1.0f};
  target->TexID = textureID;
  target++;

  target->Position = {x, y + size};
  target->Color = {0.18f, 0.6f, 0.96f, 1.0f};
  target->TexCoords = {0.0f, 1.0f};
  target->TexID = textureID;
  target++;

  return target;
}

void TestDynamicBatchRendering::OnGUIRender() {
  GUI &gui = GetGUI();
  gui.ShowDrag2f("Quad Position", m_QuadPosition, 0.1f);
  gui.ShowDrag2f("Translation", &m_Translation.x, 0.1f);
  gui.ShowFramerate();
}

}  // namespace test