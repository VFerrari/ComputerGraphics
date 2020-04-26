#ifndef DRAW_H
#define DRAW_H

#include "global.h"
#include <vector>

class Scene{
public:
    Scene(uint16_t width, uint16_t height);
    
    // Get/Set
    glm::mat4 getCamera(void);
    bool getCameraMode(void);
    uint8_t getShape(uint8_t index, std::vector<float> *dest);
    void setCameraMode(void);
    void setAllShapes(void);
    void setCurrentShape(uint8_t shape);
    
    void bufferShape(GLuint *vbo, GLuint *vao, uint8_t shape, std::vector<float> *points);
    
    // Input handler
    static void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
    
private:
    void createFigures(void);
    void createCamera(uint16_t width, uint16_t height);

    // 3D Shapes
    std::vector<float> shapes[9];
    uint8_t curr_shape=0;
    bool all_shapes = false;
    
    // Camera.
    bool camera_mode = true;
    glm::mat4 camera_pos;
};


#endif // DRAW_H
