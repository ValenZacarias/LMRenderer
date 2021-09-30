#include <glm/glm.hpp> 
#include <vector>
#include <glm/gtc/matrix_transform.hpp> 

#include "Globals.h"


//int screenWidth = 640;
//int screenHeight = 360;
int screenWidth = 1280;
int screenHeight = 720;

float deltaTime = 0.0f;
glm::vec3 lightPosition = glm::vec3(0.0f, 1.0f, 1.0f);
int DataStructureCurrentIndex = 0;

//float CAMERA_SPEED = 10.0f;
float CAMERA_SPEED = 0.1f;
bool firstMouse = true;