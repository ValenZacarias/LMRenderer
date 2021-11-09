#pragma once
#include <glm/glm.hpp> 
using namespace std;


extern int screenWidth;
extern int screenHeight;

extern float deltaTime;
extern glm::vec3 lightPosition;

extern int DataStructureCurrentIndex;

extern float CAMERA_SPEED;
extern float MESH_SCALE;
extern bool	PERSPECTIVE_CAM;
extern bool firstMouse;

extern glm::vec3 BB_COLOR_HIDDEN;
extern glm::vec3 BB_COLOR_VISIBLE;