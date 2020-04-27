#include "shaders.h"

inline float squaredLength( const std::vector<float>& input){
    float lengh=0.f;
    for(auto it=input.begin(); it!= input.end(); ++it)
    {

        lengh += (*it) * (*it);
    }
    return lengh;
}

inline void normalize(const std::vector<float>& input, std::vector<float>& output){

    float length = std::sqrt(squaredLength(input));

    output = input;
    if(length == 1.f || length == 0.f) { return; }

    float scaleFactor = 1.f/length;

    for(auto it=output.begin(); it!= output.end(); ++it)
    {
        *it *= scaleFactor;
    }
}

Shaders::Shaders(void){

    const char* vertex_shader =
            "#version 400\n"
            "layout (location=0) in vec3 position;"
            "layout (location=1) in vec3 color;"
            "out vec3 vColor;"
            "out vec3 vNormal;"
            "uniform mat4 MVP;"
            "void main() {"
            " gl_Position = MVP*vec4(position, 5.0f);"
            " vColor = color;"
            //" vNormal = normal;"
            "}";

    const char* fragment_shader =
            "#version 400\n"
            //"uniform vec3 vColor;"
            "uniform vec3 vNormal;"
            "in vec3 vColor;"
            //"in vec3 vNormal;"
            "out vec4 fColor;"
            "void main() {"
            "   vec3 lightColor = vec3(0.5f, 0.5f, 0.5f);"
            "   vec3 norm = normalize(vNormal);"
            "   float cos = dot(norm, lightColor);"
            "   cos = clamp(cos, 0.0, 1.0);"
            "   fColor = vec4(vColor * cos, 1.0f);"
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
