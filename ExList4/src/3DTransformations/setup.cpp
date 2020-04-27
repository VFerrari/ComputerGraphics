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

    // Window hints.
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    
    // Accept fragment if it is closer to the camera than the former one
    glDepthFunc(GL_LESS);
}
