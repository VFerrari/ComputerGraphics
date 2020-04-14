#ifndef SHADERS_H
#define SHADERS_H

#include "global.h"

class Shaders{
public:
    Shaders();
    void destroyShaders(void);
    GLuint getProgram(void);
private:
    GLuint vs, fs, shader_program;
};

#endif // SHADERS_H
