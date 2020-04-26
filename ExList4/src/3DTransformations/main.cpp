#include <memory>
#include <vector>

#include "global.h"
#include "setup.h"
#include "shaders.h"
#include "draw.h"

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
    glDebugMessageCallback( MessageCallback, 0);

    if(window == nullptr)
        return -1;

    // Creates scene object.
    Scene *scene = new Scene();

    // Vertex buffers and arrays.
    GLuint vbo, vao = 0;

    // Shaders
    Shaders *sh;
    GLuint sh_prog;
    GLuint MVP_ID;

    // Current shape.
    std::vector<float> points;

    // Create OpenGL shaders.
    sh = new Shaders();

    glGenBuffers(1, &vbo); //generate buffer object names, 1 = number of buffer object names to be generated.

    scene->getShape(0, &points);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //bind a named buffer object
    glBufferData(GL_ARRAY_BUFFER, points.size()*6*sizeof(GLfloat), &points[0], GL_STATIC_DRAW); //creates and initializes a buffer object's data store

    glGenVertexArrays(1, &vao); // generate vertex array object names
    glBindVertexArray(vao); //bind a vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind a named buffer object
    glEnableVertexAttribArray(0); //Enable or disable a generic vertex attribute array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr); //define an array of generic vertex attribute data
    glEnableVertexAttribArray(1); //Enable or disable a generic vertex attribute array
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<const void *>(3*sizeof(float))); //define an array of generic vertex attribute data

    sh_prog = sh->getProgram();

    while(glfwWindowShouldClose(window) == 0 ){

        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Camera
        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        model      = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH)/HEIGHT, 0.1f, 100.0f);
        glm::mat4 mvp = projection*view*model;

        MVP_ID = glGetUniformLocation(sh_prog, "MVP");
        glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);

        // draw points 0-3 from the currently bound VAO with current in-use shader

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(points.size()/6));
        //glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL );

        // update other events like input handling
        glfwPollEvents();

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }

    // Close OpenGL window, terminate GLFW and delete scene.
    glfwTerminate();
    delete scene;

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    sh->destroyShaders();

    return 0;
}
