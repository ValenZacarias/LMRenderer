#pragma once

extern float deltaTime;

////Camera settings
extern float FOV;
extern bool firstMouse;

extern const float cameraSpeed;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern glm::vec3 direction; 

////Camera direction settings
extern float pitch;
extern float yaw;
extern float lastX; 
extern float lastY;
extern float xoffset;
extern float yoffset;
extern const float sensitivity;
//bool firstMouse = true;
extern glm::vec3 front;