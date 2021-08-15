#include "TestClearColor.h"

#include "Renderer.h"

namespace test {
TestClearColor::TestClearColor(GUI &gui)
    : m_ClearColor{0.2f, 0.3f, 0.8f, 1.0f}, Test(gui) {}

void TestClearColor::OnRender() {
  GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
                      m_ClearColor[3]));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColor::OnGUIRender() {
  GetGUI().ShowColorEdit4("Clear Color", m_ClearColor);
}
}  // namespace test