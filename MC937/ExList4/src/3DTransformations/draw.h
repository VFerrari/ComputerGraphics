#ifndef DRAW_H
#define DRAW_H

#include "global.h"
#include <vector>
#include <array>
#include <cmath>

#define PI 3.14159265

class Scene{
public:
    Scene(uint16_t width, uint16_t height, GLuint *vbo, GLuint *vao);
    
    // Getters
    glm::mat4 getCamera(void);
    bool getCameraMode(void);
    uint8_t getShape(uint8_t index, std::vector<float> *dest);
    uint8_t getCurrentShape(void);

    // Setters
    void setCameraMode(void);
    void setCurrentShape(uint8_t shape);
  
    // Camera
    void moveCamera(float factorX, float factorY, float factorZ);
        
    // Buffer
    void bufferShape(uint8_t shape);
    
    // Transform
    void scale(float factorX, float factorY, float factorZ);
    void translate(float factorX, float factorY, float factorZ);
    void rotate(int angle, uint8_t axis);
    
private:
    // Init
    void createFigures(void);
    void createCamera(uint16_t width, uint16_t height);

    // Transform
    void scaleOneShape(float factorX, float factorY, float factorZ, uint8_t shape);
    void translateOneShape(float factorX, float factorY, float factorZ, uint8_t shape);
    void rotateOneShape(int angle, uint8_t axis, uint8_t shape);
    
    // Draw
    void drawPyramid(float width, float height, float depth, float start_x, float start_y, float start_z);
    void drawTetrahedron(float width, float height, float depth, float start_x, float start_y, float start_z);
    void drawParallelepiped(float width, float height, float depth, float start_x, float start_y, float start_z);
    void drawHourglass(float width, float height, float depth, float start_x, float start_y, float start_z);
    void drawIcosahedron(std::array<float,3> center, float radius);

    // Center
    void findCenter(uint8_t shape);

    // 3D Shapes
    std::vector<float> shapes[SHAPES];
    std::array<float,3> center[SHAPES];
    uint8_t curr_shape=0;
    
    // Camera.
    bool camera_mode = true;
    glm::mat4 camera_pos;
    
    // VAO and VBO
    GLuint *vbo, *vao;
    
};

#endif // DRAW_H
