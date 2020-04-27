#include "input.h"

void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods){
    Scene *scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    bool update = false;
    uint8_t curr_shape;
    
    // Key pressed
    if(action == GLFW_PRESS){
        switch(key){
            
            // Escape: close window.
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, 1);
                break;
            
            // c: enable/disable camera mode
            case GLFW_KEY_C:
                scene->setCameraMode();
                break;
            
            // up: move camera up or translate shape up
            case GLFW_KEY_UP:
                if(scene->getCameraMode())
                    scene->moveCamera(0.f, -0.3f);
                else{
                    scene->translate(0.f, 0.2f, 0.f); 
                    update = true;
                }
                break;
            
            // down: move camera down or translate shape down
            case GLFW_KEY_DOWN:
                if(scene->getCameraMode())
                    scene->moveCamera(0.f, 0.3f);
                else{
                    scene->translate(0.f, -0.2f, 0.f);
                    update = true;
                }
                break;
            
            // left: move camera left or translate shape left
            case GLFW_KEY_LEFT:
                if(scene->getCameraMode())
                    scene->moveCamera(0.3f, 0.f);
                else{
                    scene->translate(-0.2f, 0.f, 0.f);
                    update = true;
                }
                break;
                
            // right: move camera right or translate shape right
            case GLFW_KEY_RIGHT:
                if(scene->getCameraMode())
                    scene->moveCamera(-0.3f, 0.f);
                else{
                    scene->translate(0.2f, 0.f, 0.f);
                    update = true;
                }
                break;
            
            // 1-9: select shape 1-9.
            case GLFW_KEY_1:
            case GLFW_KEY_2:
            case GLFW_KEY_3:
            case GLFW_KEY_4:
            case GLFW_KEY_5:
            case GLFW_KEY_6:
            case GLFW_KEY_7:
            case GLFW_KEY_8:
            case GLFW_KEY_9:
                scene->setCurrentShape(scancode-10);
                printf("%d\n", scancode-10);
                break;
            
            // 0: select all shapes
            case GLFW_KEY_0:
                scene->setCurrentShape(SHAPES);
                printf("All\n");
                break;
            
            // x: scale down or up in X
            case GLFW_KEY_X:
                if(mods & GLFW_MOD_SHIFT)
                    scene->scale(1.1f, 1.f, 1.f);
                else
                    scene->scale(0.9f, 1.f, 1.f);
                update = true;
                break;
            
            // y: scale down or up in Y
            case GLFW_KEY_Y:
                if(mods & GLFW_MOD_SHIFT)
                    scene->scale(1.f, 1.1f, 1.f);
                else
                    scene->scale(1.f, 0.9f, 1.f);
                update = true;
                break;
                
            // z: scale down or up in Z
            case GLFW_KEY_Z:
                if(mods & GLFW_MOD_SHIFT)
                    scene->scale(1.f, 1.f, 1.1f);
                else
                    scene->scale(1.f, 1.f, 0.9f);
                update = true;
                break;
            
            // s: rotate through X
            case GLFW_KEY_S:
                scene->rotate(15, 1);
                update = true;
                break;
            
            // q: rotate through Y
            case GLFW_KEY_Q:
                scene->rotate(15, 2);
                update = true;
                break;
                
            // a: rotate through Z
            case GLFW_KEY_A:
                scene->rotate(15, 3);
                update = true;
                break;
                
            // ,: move the shape forward
            case GLFW_KEY_COMMA:
                scene->translate(0.f, 0.f, 0.2f);
                update = true;
                break;
            
            // .: move the shape backward
            case GLFW_KEY_PERIOD:
                scene->translate(0.f, 0.f, -0.2f);
                update = true;
                break;
                    
            default:
                break;
        }
    }
    
    // Update buffers if needed.
    if(update){
        curr_shape = scene->getCurrentShape();
        
        // If all shapes
        if(curr_shape < SHAPES)
            scene->bufferShape(curr_shape);
        else
            for(int i=0; i<SHAPES; i++)
                scene->bufferShape(i);
    }
}
