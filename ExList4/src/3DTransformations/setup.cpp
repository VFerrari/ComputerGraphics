#include "setup.h"


GLFWwindow* mainSetup(uint16_t width, uint16_t height, const char* name){

    if (!initGLFW())
        return 0;

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr,nullptr);

    if(window == nullptr){
        std::cerr<<"Failed to open GLFW window"<<std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!initGLEW())
        return nullptr;

    // Configure OpenGL
    configGL(window);

    return window;
}

int initGLFW(void){

    // Initialize GLFW
    glewExperimental = true; // Needed for core profile
    if( !glfwInit()){
        std::cerr<<"Failed to initialize GLFW\n"<<std::endl;
        return 0;
    }

    return 1;
}

int initGLEW(void){

    // Initialize GLEW
    if (glewInit() != GLEW_OK){
        std::cerr<<"Failed to initialize GLEW"<<std::endl;
        glfwTerminate();
        return 0;
    }

    return 1;
}

void configGL(GLFWwindow* window){

    // Ensure we can capture the keys being pressed.
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Black background
    glClearColor(0.f, 0.f, 0.f, 1.0f);

    // Background far away.
    glClearDepth(1.0f);
}

/**
 * Creating shaders and linking.
 * @brief createShaders
 */
void createShaders(void){

    const char* vertex_shader =
            "#version 400\n"
            "in vec3 vp;"
            "void main() {"
            "  gl_Position = vec4(vp, 1.0);"
            "}";

    const char* fragment_shader =
            "#version 400\n"
            "out vec4 frag_colour;"
            "void main() {"
            "  frag_colour = vec4(0.5, 0.0, 0.0, 1.0);"
            "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER); //GL_VERTEX_SHADER is a shader that is intended to run on the programmable vertex processor.
    glShaderSource(vs, 1, &vertex_shader, nullptr); //Replaces the source code in a shader object
    glCompileShader(vs); //Compiles a shader object

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER); //GL_FRAGMENT_SHADER is a shader that is intended to run on the programmable fragment processor.
    glShaderSource(fs, 1, &fragment_shader, nullptr); // Replaces the source code in a shader object
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram();//Creates a program object, A program object is an object to which shader objects can be attached.
    glAttachShader(shader_program, fs); // Attaches a shader object to a program object
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);
    glUseProgram(shader_program); //Installs a program object as part of current rendering state
}
