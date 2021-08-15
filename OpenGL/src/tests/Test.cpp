#include "Test.h"

namespace test {

TestMenu::TestMenu(GUI &gui, Test *&currentTestPointer)
    : m_CurrentTest(currentTestPointer), Test(gui) {}

void TestMenu::OnGUIRender() {
  for (auto &test : m_Tests) {
    if (GetGUI().Button(test.first.c_str())) {
      m_CurrentTest = test.second();
    }
  }
}

}  // namespace test