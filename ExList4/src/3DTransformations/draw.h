#ifndef DRAW_H
#define DRAW_H

#include "global.h"
#include <vector>

class Scene{
public:
    Scene();
    uint8_t getShape(uint8_t index, std::vector<float> *dest);

private:
    void createFigures(void);

    // 3D Shapes
    std::vector<float> shapes[9];
};


#endif // DRAW_H
