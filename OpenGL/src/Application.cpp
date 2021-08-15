// STL
#include <iostream>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Local
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// Constants
#define HEIGHT 960.0f
#define WIDTH 540.0f
#define ASPECT_RATIO (HEIGHT / WIDTH)
#define ASPECT_RATIO_H ASPECT_RATIO *HEIGHT
#define ASPECT_RATIO_W ASPECT_RATIO *WIDTH

int main(void) {
  GLFWwindow *window;

  /* Initialize the GLFW library */
  if (!glfwInit()) return -1;

  /* GLFW hints */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(HEIGHT, WIDTH, "Hello World", NULL, NULL);
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
        100.0f, 100.0f, 0.0f, 0.0f,  // 0
        200.0f, 100.0f, 1.0f, 0.0f,  // 1
        200.0f, 200.0f, 1.0f, 1.0f,  // 2
        100.0f, 200.0f, 0.0f, 1.0f   // 3
    };

    unsigned int indices[] = {
        0, 1, 2,  // 1st triangle
        2, 3, 0   // 2nd triangle
    };

    /* Blending */
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    /* Vertex Arrays and Buffers */
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    /* Vertex Attributes */
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    /* Index Buffers */
    IndexBuffer ib(indices, 6);

    /* Shaders */
    Shader shader("../res/shaders/Texture.shader");
    shader.Bind();

    /* Initialize color values. */
    float r = 0.0f;
    float increment = 0.05f;
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    /* MVP */
    glm::mat4 proj = glm::ortho(0.0f, HEIGHT, 0.0f, WIDTH, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);

    /* Get Texture */
    Texture texture("../res/textures/ChernoLogo.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

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