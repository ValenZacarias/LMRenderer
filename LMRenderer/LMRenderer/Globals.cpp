#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 

#include "Globals.h"

//int screenWidth = 640;
//int screenHeight = 360;
int screenWidth = 1280;
int screenHeight = 720;

float deltaTime = 0.0f;
bool firstMouse = true;
glm::vec3 lightPosition = glm::vec3(3.0f, 3.0f, 3.0f);
int DataStructureCurrentIndex = 0;