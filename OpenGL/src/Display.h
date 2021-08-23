#pragma once

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Display {
 private:
  GLFWwindow *m_Window;

 public:
  Display(const char *windowName);
  ~Display();
  GLFWwindow *GetWindow() const { return m_Window; }
  void Poll();
  void SwapBuffers();
  bool ShouldClose();
};