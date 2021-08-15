// STL
#include <iostream>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Local
#include "GUI.h"
#include "Renderer.h"

// Tests
#include "tests/TestClearColor.h"
#include "tests/TestBlinkingSquare.h"
#include "tests/TestTexture2D.h"

// Constants
#include "Settings.h"

int main(void) {
  GLFWwindow *window;

  /* Initialize the GLFW library */
  if (!glfwInit()) return -1;

  /* GLFW hints */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(WIDTH, HEIGHT, "Tests", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Set swap interval */
  glfwSwapInterval(1);

  /* Initialize GLEW */
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    glfwTerminate();
    return 0;
  }

  /* Print the version */
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  {
    /* Creating renderer */
    Renderer renderer;

    /* Creating GUI */
    GUI gui(window);

    /* Creating Test Menu */
    test::Test *currentTest = nullptr;
    test::TestMenu *testMenu = new test::TestMenu(gui, currentTest);
    currentTest = testMenu;

    /* Registering Tests */
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestBlinkingSquare>("Blinking Square");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

    while (!glfwWindowShouldClose(window)) {
      renderer.Clear();

      /* GUI Frame */
      gui.NewFrame();

      /* Running Test */
      if (currentTest) {
        currentTest->OnUpdate(0.0f);
        currentTest->OnRender();
        gui.Begin("Test");
        if (currentTest != testMenu && gui.Button("<-")) {
          delete currentTest;
          currentTest = testMenu;
        }
        currentTest->OnGUIRender();
        gui.End();
      }

      /* Render GUI */
      gui.Render();

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
    if (currentTest != testMenu) delete testMenu;
    delete currentTest;
  }

  glfwTerminate();
  return 0;
}