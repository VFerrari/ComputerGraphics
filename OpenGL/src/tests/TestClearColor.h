#pragma once

#include "Test.h"

namespace test {

class TestClearColor : public Test {
 private:
  float m_ClearColor[4];

 public:
  TestClearColor(GUI &gui);

  void OnRender() override;
  void OnGUIRender() override;
};

}  // namespace test