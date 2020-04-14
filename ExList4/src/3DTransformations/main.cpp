#include <memory>
#include <vector>

#include "global.h"
#include "setup.h"
#include "shaders.h"
#include "draw.h"

int main(){

    // Setup for GLFW, GLEW and OpenGL. Creates window.
    GLFWwindow *window = mainSetup(600, 600, "3D Transformations");
    if(window == nullptr)
        return -1;

    // Creates scene object.
    Scene *scene = new Scene();

    // Vertex buffers and arrays.
    GLuint vbo, vao = 0, index_vbo;

    // Current shape.
    std::vector<float> points;

    float colors[] = {  // Colors of the 5 vertices in RGBA
          0.0f, 0.0f, 1.0f, 1.0f,  // 0. blue
          0.0f, 1.0f, 0.0f, 1.0f,  // 1. green
          0.0f, 0.0f, 1.0f, 1.0f,  // 2. blue
          0.0f, 1.0f, 0.0f, 1.0f,  // 3. green
          1.0f, 0.0f, 0.0f, 1.0f   // 4. red
    };

    float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };


    GLuint indices[] = { // Vertex indices of the 4 Triangles
          2, 4, 3,   // front face (CCW)
          1, 4, 2,   // right face
          0, 4, 1,   // back face
          4, 0, 3    // left face
    };

    // Create OpenGL shaders.
    Shaders *sh = new Shaders();
    GLuint sh_prog = sh->getProgram();

    glGenBuffers(1, &vbo); //generate buffer object names, 1 = number of buffer object names to be generated.

    scene->getShape(0, &points);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //bind a named buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //creates and initializes a buffer object's data store

    glGenBuffers(1, &index_vbo);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW );

    glGenVertexArrays(1, &vao); // generate vertex array object names
    glBindVertexArray(vao); //bind a vertex array object
    glEnableVertexAttribArray(0); //Enable or disable a generic vertex attribute array

    glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind a named buffer object
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //define an array of generic vertex attribute data




    while(glfwWindowShouldClose(window) == 0 ){

        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT);

        // draw points 0-3 from the currently bound VAO with current in-use shader
        //glDrawArrays(GL_TRIANGLES, 0, 5);
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL );

        // update other events like input handling
        glfwPollEvents();

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }

    // Close OpenGL window, terminate GLFW and delete scene.
    glfwTerminate();
    delete scene;

    glDeleteBuffers(1, &index_vbo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    sh->destroyShaders();

    return 0;
}
