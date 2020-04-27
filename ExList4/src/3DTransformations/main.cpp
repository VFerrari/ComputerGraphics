#include <memory>
#include <vector>
#include <array>
#include <ctime>

#include "global.h"
#include "setup.h"
#include "shaders.h"
#include "draw.h"
#include "input.h"

#define WIDTH 600
#define HEIGHT 600

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message);
}

int main(){

    // Setup for GLFW, GLEW and OpenGL. Creates window.
    GLFWwindow *window = mainSetup(WIDTH, HEIGHT, "3D Transformations");

    // Debug
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    if(window == nullptr)
        return -1;

    // Vertex buffers and arrays.
    GLuint vbo[SHAPES], vao[SHAPES];

    // Creates scene object: camera, shapes.
    srand (static_cast <unsigned> (time(0)));
    Scene *scene = new Scene(WIDTH, HEIGHT, vbo, vao);

    // Shaders
    Shaders *sh = new Shaders();
    GLuint sh_prog;
    GLuint mvp_loc;
    int norm_loc;
    
    // Current shape.
    std::vector<float> points;
    
    // Camera
    glm::mat4 mvp;
    

    // Generate vertex arrays and buffers.
    glGenBuffers(SHAPES, vbo); 
    glGenVertexArrays(SHAPES, vao);

    // Insert shapes into buffers
    for (uint8_t i=0; i<SHAPES; i++)
        scene->bufferShape(i);
    
    // Get shader attributes
    sh_prog = sh->getProgram();
    norm_loc = glGetUniformLocation(sh_prog, "vNormal");
    mvp_loc = glGetUniformLocation(sh_prog, "MVP");
    
    // Set normal
    glUniform3f(norm_loc, 1.0f, 1.0f, 1.0f);
    
    // Set key callback
    glfwSetWindowUserPointer(window, scene);
    glfwSetKeyCallback(window, handleInput);

    while(glfwWindowShouldClose(window) == 0){

        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Camera
        mvp = scene->getCamera();
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

        // Draw shapes
        for(int i=0; i<SHAPES; i++){
            scene->getShape(i, &points);
            glBindVertexArray(vao[i]);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(points.size()/6));
        }

        // Put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
        
        // Wait for input.
        glfwWaitEvents();
    }

    // Close OpenGL window, terminate GLFW.
    glfwTerminate();
    
    // Delete VBOs and VAOs
    glDeleteBuffers(SHAPES, vbo);
    glDeleteVertexArrays(SHAPES, vao);

    // Delete shaders.
    sh->destroyShaders();
    
    // Deleting heap objects.
    delete scene;
    delete sh;

    return 0;
}
