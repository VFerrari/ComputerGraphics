#include "draw.h"

Scene::Scene(void){
    createFigures();
}


void Scene::createFigures(void){
    // Pyramid
    shapes[0] = { // 5 vertices of the pyramid in (x,y,z)
          -1.0f, -1.0f, -1.0f,  // 0. left-bottom-back
           1.0f, -1.0f, -1.0f,  // 1. right-bottom-back
           1.0f, -1.0f,  1.0f,  // 2. right-bottom-front
          -1.0f, -1.0f,  1.0f,  // 3. left-bottom-front
           0.0f,  1.0f,  0.0f   // 4. top
    };

    // Cube
    shapes[1] = {

    };
}

uint8_t Scene::getShape(uint8_t index, std::vector<float> *dest){
    if (index > 8){
        std::cerr << "Shape index out of range!\n" << std::endl;
        return 0;
    }

    *dest = shapes[index];
    return 1;
}
