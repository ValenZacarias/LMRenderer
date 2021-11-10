#include <glm/glm.hpp> 
#include <vector>
#include <glm/gtc/matrix_transform.hpp> 

#include "Globals.h"


//int screenWidth = 640;
//int screenHeight = 360;
int screenWidth = 1280;
int screenHeight = 720;

float deltaTime = 0.0f;
glm::vec3 lightPosition = glm::vec3(0.0f, 10.0f, 5.0f);
int DataStructureCurrentIndex = 0;

float CAMERA_SPEED = 6.25f;
float MOUSE_SENSITIVITY = 0.1f;

bool firstMouse = true;
bool PERSPECTIVE_CAM = true;
bool CAM_ENABLED = true;

float MESH_SCALE = 1.0f;

glm::vec3 BB_COLOR_HIDDEN = glm::vec3(0.1f, 0.3f, 0.9f);
glm::vec3 BB_COLOR_VISIBLE = glm::vec3(0.1f, 0.9f, 0.2f);