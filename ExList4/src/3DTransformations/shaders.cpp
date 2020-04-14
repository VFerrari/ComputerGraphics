#include "shaders.h"

Shaders::Shaders(void){

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

    //GL_VERTEX_SHADER: shader that is intended to run on the programmable vertex processor.
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    //GL_FRAGMENT_SHADER: shader that is intended to run on the programmable fragment processor.
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    //Creates a program object: object to which shader objects can be attached.
    shader_program = glCreateProgram();

    // Attaches a shader object to a program object
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glBindFragDataLocation(shader_program, 0, "outColor");
    glLinkProgram(shader_program);

    //Installs a program object as part of current rendering state
    glUseProgram(shader_program);
}

GLuint Shaders::getProgram(void){
    return shader_program;
}

void Shaders::destroyShaders(void){
    glDeleteProgram(shader_program);
    glDeleteShader(fs);
    glDeleteShader(vs);
}
