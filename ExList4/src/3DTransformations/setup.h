#ifndef SETUP_H
#define SETUP_H

#include "global.h"

GLFWwindow* mainSetup(uint16_t width, uint16_t height, const char* name);
int initGLFW(void);
int initGLEW(void);
void createShaders(void);
void configGL(GLFWwindow* window);

#endif // SETUP_H
