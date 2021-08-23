// STL
#include <iostream>
#include <stdexcept>

// OpenGL
#include <GL/glew.h>

// Local
#include "Display.h"
#include "GUI.h"
#include "Renderer.h"

// Tests
#include "tests/TestBatchRendering.h"
#include "tests/TestBlinkingSquare.h"
#include "tests/TestClearColor.h"
#include "tests/TestDynamicBatchRendering.h"
#include "tests/TestTexture2D.h"

int main(void) {
  try {
    // Initialize display
    Display display("Tests");

    /* Initialize GLEW */
    if (glewInit() != GLEW_OK) {
      std::cerr << "Failed to initialize GLEW" << std::endl;
      return -1;
    }

    /* Print the version */
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    /* Creating renderer */
    Renderer renderer;

    /* Creating GUI */
    GUI gui(display.GetWindow());

    /* Creating Test Menu */
    test::Test *currentTest = nullptr;
    test::TestMenu *testMenu = new test::TestMenu(gui, currentTest);
    currentTest = testMenu;

    /* Registering Tests */
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestBlinkingSquare>("Blinking Square");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<test::TestBatchRendering>("Batch Rendering");
    testMenu->RegisterTest<test::TestDynamicBatchRendering>(
        "Dynamic Batch Rendering");

    while (!display.ShouldClose()) {
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

      display.SwapBuffers();
      display.Poll();
    }
    if (currentTest != testMenu) delete testMenu;
    delete currentTest;

    return 0;

  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
}