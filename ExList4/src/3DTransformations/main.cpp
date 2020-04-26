#include <memory>
#include <vector>
#include <array>

#include "global.h"
#include "setup.h"
#include "shaders.h"
#include "draw.h"

#define WIDTH 600
#define HEIGHT 600

#define SHAPES 2

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

    // Creates scene object: camera, shapes.
    Scene *scene = new Scene(WIDTH, HEIGHT);

    // Vertex buffers and arrays.
    GLuint vbo[SHAPES], vao[SHAPES];

    // Shaders
    Shaders *sh = new Shaders();
    GLuint sh_prog;
    GLuint mvp_loc;
    int vertex_color_loc;
    
    // Current shape.
    std::vector<float> points;
    
    // Camera
    glm::mat4 mvp;
    

	// Generate vertex arrays and buffers.
    glGenBuffers(SHAPES, vbo); 
    glGenVertexArrays(SHAPES, vao);

	// Insert shapes into buffers
	for (uint8_t i=0; i<SHAPES; i++)
		scene->bufferShape(vbo, vao, i, &points);
	
	// Get shader attributes
    sh_prog = sh->getProgram();
	vertex_color_loc = glGetUniformLocation(sh_prog, "vColor");
    mvp_loc = glGetUniformLocation(sh_prog, "MVP");

	// Get camera
	mvp = scene->getCamera();
	
	// Set key callback
	glfwSetWindowUserPointer(window, scene);
	glfwSetKeyCallback(window, scene->handleInput);

	// Colors (TEMP)
	float colors[SHAPES][3] = { {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} };

    while(glfwWindowShouldClose(window) == 0){

        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // MVP
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

        // Draw shapes
        for(int i=0; i<SHAPES; i++){
			glUniform3f(vertex_color_loc, colors[i][0], colors[i][1], colors[i][2]);
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
