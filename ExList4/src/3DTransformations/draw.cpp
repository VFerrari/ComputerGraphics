#include "draw.h"

Scene::Scene(void){
    createFigures();
}


void Scene::createFigures(void){
    // Pyramid
    shapes[0] = {
        -5.0f, 5.0f,  0.0f, 1.f, 0.f, 0.f,
        -5.0f, 3.0f,  1.0f, 1.f, 0.f, 0.f,
        -4.0f, 3.0f,  1.0f, 1.f, 1.f, 0.f,

        -5.0f, 5.0f,  0.0f, 1.f, 0.f, 0.f,
        -4.0f, 3.0f,  1.0f, 1.f, 0.f, 0.f,
        -4.0f, 3.0f, -1.0f, 1.f, 0.f, 1.f,

        -5.0f, 5.0f,  0.0f, 1.f, 0.f, 0.f,
        -4.0f, 3.0f, -1.0f, 1.f, 1.f, 0.f,
        -6.0f, 3.0f, -1.0f, 1.f, 0.f, 0.f,

        -5.0f, 5.0f,  0.0f, 1.f, 0.f, 0.f,
        -6.0f, 3.0f, -1.0f, 1.f, 0.f, 0.f,
        -6.0f, 3.0f,  1.0f, 1.f, 1.f, 0.f,
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
