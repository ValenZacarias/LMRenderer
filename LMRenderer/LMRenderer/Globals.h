#pragma once
#include <glm/glm.hpp> 
using namespace std;

extern int VIS_ID_COUNTER;

extern int screenWidth;
extern int screenHeight;

extern float deltaTime;
extern glm::vec3 LIGHT_POS;

extern int DataStructureCurrentIndex;

extern bool	PERSPECTIVE_CAM;
extern bool	CAM_ENABLED;
extern float CAMERA_SPEED;

extern float MOUSE_SENSITIVITY;
extern bool firstMouse;
extern bool DYNAMIC_LOADING_ENABLED;
extern int DYNAMIC_LOADING_LIMIT;

extern int ZONE_STARTING_SAMPLE_SIZE;
extern int ZONE_SAMPLE_GROW_FACTOR;

extern float MESH_SCALE;
extern float FRUSTRUM_SCALE;

extern glm::vec3 BB_COLOR_HIDDEN;
extern glm::vec3 BB_COLOR_VISIBLE;