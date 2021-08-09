// STL
#include <iostream>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Local
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

int main(void) {
  GLFWwindow *window;

  /* Initialize the GLFW library */
  if (!glfwInit()) return -1;

  /* GLFW hints */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
    /* Shape array */
    float positions[] = {
        -0.5f, -0.5f,  // 0
        0.5f,  -0.5f,  // 1
        0.5f,  0.5f,   // 2
        -0.5f, 0.5f    // 3
    };

    unsigned int indices[] = {
        0, 1, 2,  // 1st triangle
        2, 3, 0   // 2nd triangle
    };

    /* Vertex Arrays and Buffers */
    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    /* Vertex Attributes */
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    /* Index Buffers */
    IndexBuffer ib(indices, 6);

    /* Shaders */
    Shader shader("../res/shaders/Basic.shader");
    shader.Bind();

    /* Initialize color values. */
    float r = 0.0f;
    float increment = 0.05f;
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    /* Clear everything (for testing vao). */
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    /* Creating renderer */
    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.Clear();

      /* Draw with a certain color. */
      shader.Bind();
      shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
      renderer.Draw(va, ib, shader);

      // Change color.
      if (r > 1.0f)
        increment = -0.05f;
      else if (r < 0.0f)
        increment = 0.05f;

      r += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}