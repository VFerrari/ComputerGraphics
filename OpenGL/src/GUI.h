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

  void Begin(const char *name);
  void End();

  void ShowFramerate();
  void ShowSlider3f(const char *name, float *target, float min, float max);
  void ShowColorEdit4(const char *name, float *target);

  bool Button(const char *name);
};