#pragma once

// STL
#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Local
#include "GUI.h"
#include "Renderer.h"

namespace test {

class Test {
 private:
  GUI &m_gui;

 public:
  Test(GUI &gui) : m_gui(gui) {}
  virtual ~Test() { GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); }

  virtual void OnUpdate(float deltaTime) {}
  virtual void OnRender() {}
  virtual void OnGUIRender() {}

  GUI &GetGUI() { return m_gui; }
};

class TestMenu : public Test {
 public:
  TestMenu(GUI &gui, Test *&currentTestPointer);

  void OnGUIRender() override;

  template <typename T>
  void RegisterTest(const std::string &name) {
    std::cout << "Registering test " << name << std::endl;
    GUI &gui = GetGUI();
    m_Tests.push_back(std::make_pair(name, [&]() { return new T(gui); }));
  }

 private:
  Test *&m_CurrentTest;
  std::vector<std::pair<std::string, std::function<Test *()>>> m_Tests;
};

}  // namespace test