#ifndef INPUT_H
#define INPUT_H

#include "global.h"
#include "draw.h"

void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void changeHighlight(Scene *scene, uint8_t shape);

#endif // INPUT_H
