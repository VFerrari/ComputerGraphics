#include "draw.h"

Scene::Scene(uint16_t width, uint16_t height){	
    createFigures();
    createCamera(width, height);
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

    // Box
    shapes[1] =  {
		-1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,
		-1.0f,-1.0f, 1.0f, 0.f, 0.f, 0.f,
		-1.0f, 1.0f, 1.0f, 0.f, 0.f, 0.f,

		 1.0f, 1.0f,-1.0f, 1.f, 0.f, 0.f,
		-1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,
		-1.0f, 1.0f,-1.0f, 1.f, 0.f, 1.f,

		 1.0f,-1.0f, 1.0f, 1.f, 0.f, 0.f,
		-1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,
		 1.0f,-1.0f,-1.0f, 0.f, 1.f, 0.f,

		 1.0f, 1.0f,-1.0f, 1.f, 0.f, 0.f,
		 1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,
		-1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,

		-1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,
		-1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f,
		-1.0f, 1.0f,-1.0f, 1.f, 0.f, 0.f,

		 1.0f,-1.0f, 1.0f, 1.f, 0.f, 0.f,
		-1.0f,-1.0f, 1.0f, 1.f, 0.f, 0.f,
		-1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,

		-1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f,
		-1.0f,-1.0f, 1.0f, 1.f, 0.f, 0.f,
		 1.0f,-1.0f, 1.0f, 1.f, 0.f, 0.f,

		 1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f,
		 1.0f,-1.0f,-1.0f, 1.f, 0.f, 0.f,
		 1.0f, 1.0f,-1.0f, 1.f, 0.f, 0.f,

		 1.0f,-1.0f,-1.0f, 1.f, 1.f, 0.f,
		 1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f,
		 1.0f,-1.0f, 1.0f, 1.f, 1.f, 0.f,

		 1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f,
		 1.0f, 1.0f,-1.0f, 1.f, 0.f, 0.f,
		-1.0f, 1.0f,-1.0f, 1.f, 0.f, 0.f,

		 1.0f, 1.0f, 1.0f, 1.f, 1.f, 0.f,
		-1.0f, 1.0f,-1.0f, 1.f, 0.f, 0.f,
		-1.0f, 1.0f, 1.0f, 1.f, 0.f, 1.f,

		 1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f,
		-1.0f, 1.0f, 1.0f, 0.f, 0.f, 1.f,
		 1.0f,-1.0f, 1.0f, 0.5f,0.f, 1.f
    };
}

void Scene::createCamera(uint16_t width, uint16_t height){
	glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    model      = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width)/height, 0.1f, 100.0f);
    
    camera_pos = projection*view*model;	
}

glm::mat4 Scene::getCamera(void){
	return camera_pos;
}

bool Scene::getCameraMode(void){
	return camera_mode;
}

void Scene::setCameraMode(void){
	camera_mode = !camera_mode;
}

void Scene::setAllShapes(void){
	all_shapes = !all_shapes;
}

void Scene::setCurrentShape(uint8_t shape){
	curr_shape = shape;
}

uint8_t Scene::getShape(uint8_t index, std::vector<float> *dest){
    if (index > 8){
        std::cerr << "Shape index out of range!\n" << std::endl;
        return 0;
    }

    *dest = shapes[index];
    return 1;
}

void Scene::bufferShape(GLuint *vbo, GLuint *vao, uint8_t shape, std::vector<float> *points){
		
	// Get shape.
	getShape(shape, points);
	
	// Bind a VBO and initialize data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[shape]);
	glBufferData(GL_ARRAY_BUFFER, points->size()*6*sizeof(GLfloat), (*points).data(), GL_STATIC_DRAW); 

	// Bind a VAO, and set its attributes.
	glBindVertexArray(vao[shape]);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr); 
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<const void *>(3*sizeof(float))); 
}


void Scene::handleInput(GLFWwindow* window, int key, int scancode, int action, int mods){
	Scene *scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
	
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
				printf("Camera\n");
				break;
			
			// up: move camera up or translate shape up
			case GLFW_KEY_UP:
				if(scene->getCameraMode())
					printf("Move camera up\n");
				else
					printf("Move shape up\n");
				break;
			
			// down: move camera down or translate shape down
			case GLFW_KEY_DOWN:
				if(scene->getCameraMode())
					printf("Move camera down\n");
				else
					printf("Move shape down\n");
				break;
			
			// left: move camera left or translate shape left
			case GLFW_KEY_LEFT:
				if(scene->getCameraMode())
					printf("Move camera left\n");
				else
					printf("Move shape left\n");
				break;
				
			// right: move camera right or translate shape right
			case GLFW_KEY_RIGHT:
				if(scene->getCameraMode())
					printf("Move camera right\n");
				else
					printf("Move shape right\n");
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
				scene->setCurrentShape(scancode-9);
				printf("%d\n", scancode-9);
				break;
			
			// 0: select all shapes
			case GLFW_KEY_0:
				scene->setAllShapes();
				printf("%d\n", scancode-10);
				break;
			
			// x: scale down or up in X
			case GLFW_KEY_X:
				if(mods & GLFW_MOD_SHIFT)
					printf("Scale up X\n");
				else
					printf("Scale down X\n");
				break;
			
			// y: scale down or up in Y
			case GLFW_KEY_Y:
				if(mods & GLFW_MOD_SHIFT)
					printf("Scale up Y\n");
				else
					printf("Scale down Y\n");
				break;
				
			// z: scale down or up in Z
			case GLFW_KEY_Z:
				if(mods & GLFW_MOD_SHIFT)
					printf("Scale up Z\n");
				else
					printf("Scale down Z\n");
				break;
			
			// s: rotate through X
			case GLFW_KEY_S:
				printf("Rotate x-axis\n");
				break;
			
			// q: rotate through Y
			case GLFW_KEY_Q:
				printf("Rotate y-axis\n");
				break;
				
			// a: rotate through Z
			case GLFW_KEY_A:
				printf("Rotate z-axis\n");
				break;
				
			// ,: move the shape forward
			case GLFW_KEY_COMMA:
				printf("Move forward\n");
				break;
			
			// .: move the shape backward
			case GLFW_KEY_PERIOD:
				printf("Move backward\n");
				break;
					
			default:
				break;
		}
	}
}
