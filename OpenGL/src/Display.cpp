#include "Display.h"

// STL
#include <stdexcept>

Display::Display(const char *windowName, float width, float height) {
  /* Initialize the GLFW library */
  if (!glfwInit()) {
    throw std::runtime_error("GLFW Initialization failed");
  };

  /* GLFW hints */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  m_Window = glfwCreateWindow(width, height, windowName, NULL, NULL);
  if (!m_Window) {
    glfwTerminate();
    throw std::runtime_error("GLFW Initialization failed");
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_Window);

  /* Set swap interval */
  glfwSwapInterval(1);
}

Display::~Display() { glfwTerminate(); }

void Display::Poll() { glfwPollEvents(); }

void Display::SwapBuffers() { glfwSwapBuffers(m_Window); }

bool Display::ShouldClose() { return glfwWindowShouldClose(m_Window); }