#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GUI {
 private:
  GLFWwindow *GUIContext;

 public:
  GUI(GLFWwindow *window);
  ~GUI();

  void NewFrame();
  void Render();

  void ShowFramerate();
  void ShowSlider3f(const char *name, float *target, float min, float max);
};