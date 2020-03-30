#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// List of polygon vertices.
std::vector<float> listOfVertices(){
    return {
            0.0f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f,
            0.8f, 0.0f, 0.0f
           };
}

int main(){

    // Initialize GLFW
    if( !glfwInit() ){
        std::cerr<<"Failed to initialize GLFW\n"<<std::endl;
        return -1;
    }

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(600, 600, "Red Polygon", nullptr,nullptr);

    // Check if window was successfully created.
    if( window == nullptr ){
        std::cerr<<"Failed to open GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK){
        std::cerr<<"Failed to initialize GLEW"<<std::endl;
        glfwTerminate();
        return -1;
    }

    // Get Polygon
    std::vector<float> polygon = listOfVertices();

    GLuint vbo;
    GLuint vao = 0;

    // VBO
    // Generate and bind buffers (1)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Initialize buffer data.
    glBufferData(GL_ARRAY_BUFFER, polygon.size() * sizeof(float), polygon.data(), GL_STATIC_DRAW);

    // VAO
    // Generate and bind vertex arrays (1).
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Enable vertex attribute array.
    glEnableVertexAttribArray(0);

    // Define an array of generic vertex attribute data.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Polygon Color : RED.
    glColor3f(1.f, 0.f, 0.f);

    while(glfwWindowShouldClose(window) == 0 ){

        // Draw polygon from the currently bound VAO.
        glDrawArrays(GL_POLYGON, 0, polygon.size());

        // Display
        glfwSwapBuffers(window);

        // Update other events (close button)
        glfwPollEvents();
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
