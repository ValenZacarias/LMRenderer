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

//float cubeVertices[] = {
//-0.5f, -0.5f, -0.5f,
// 0.5f, -0.5f, -0.5f,
// 0.5f,  0.5f, -0.5f,
// 0.5f,  0.5f, -0.5f,
//-0.5f,  0.5f, -0.5f,
//-0.5f, -0.5f, -0.5f,
//
//-0.5f, -0.5f,  0.5f,
// 0.5f, -0.5f,  0.5f,
// 0.5f,  0.5f,  0.5f,
// 0.5f,  0.5f,  0.5f,
//-0.5f,  0.5f,  0.5f,
//-0.5f, -0.5f,  0.5f,
//
//-0.5f,  0.5f,  0.5f,
//-0.5f,  0.5f, -0.5f,
//-0.5f, -0.5f, -0.5f,
//-0.5f, -0.5f, -0.5f,
//-0.5f, -0.5f,  0.5f,
//-0.5f,  0.5f,  0.5f,
//
// 0.5f,  0.5f,  0.5f,
// 0.5f,  0.5f, -0.5f,
// 0.5f, -0.5f, -0.5f,
// 0.5f, -0.5f, -0.5f,
// 0.5f, -0.5f,  0.5f,
// 0.5f,  0.5f,  0.5f,
//
//-0.5f, -0.5f, -0.5f,
// 0.5f, -0.5f, -0.5f,
// 0.5f, -0.5f,  0.5f,
// 0.5f, -0.5f,  0.5f,
//-0.5f, -0.5f,  0.5f,
//-0.5f, -0.5f, -0.5f,
//
//-0.5f,  0.5f, -0.5f,
// 0.5f,  0.5f, -0.5f,
// 0.5f,  0.5f,  0.5f,
// 0.5f,  0.5f,  0.5f,
//-0.5f,  0.5f,  0.5f,
//-0.5f,  0.5f, -0.5f,
//};

//float cubeNormals[] =
//{
// 0.0f,  0.0f, -1.0f,
// 0.0f,  0.0f, -1.0f,
// 0.0f,  0.0f, -1.0f,
// 0.0f,  0.0f, -1.0f,
// 0.0f,  0.0f, -1.0f,
// 0.0f,  0.0f, -1.0f,
//
// 0.0f,  0.0f,  1.0f,
// 0.0f,  0.0f,  1.0f,
// 0.0f,  0.0f,  1.0f,
// 0.0f,  0.0f,  1.0f,
// 0.0f,  0.0f,  1.0f,
// 0.0f,  0.0f,  1.0f,
//
//-1.0f,  0.0f,  0.0f,
//-1.0f,  0.0f,  0.0f,
//-1.0f,  0.0f,  0.0f,
//-1.0f,  0.0f,  0.0f,
//-1.0f,  0.0f,  0.0f,
//-1.0f,  0.0f,  0.0f,
//
// 1.0f,  0.0f,  0.0f,
// 1.0f,  0.0f,  0.0f,
// 1.0f,  0.0f,  0.0f,
// 1.0f,  0.0f,  0.0f,
// 1.0f,  0.0f,  0.0f,
// 1.0f,  0.0f,  0.0f,
//
// 0.0f, -1.0f,  0.0f,
// 0.0f, -1.0f,  0.0f,
// 0.0f, -1.0f,  0.0f,
// 0.0f, -1.0f,  0.0f,
// 0.0f, -1.0f,  0.0f,
// 0.0f, -1.0f,  0.0f,
//
// 0.0f,  1.0f,  0.0f,
// 0.0f,  1.0f,  0.0f,
// 0.0f,  1.0f,  0.0f,
// 0.0f,  1.0f,  0.0f,
// 0.0f,  1.0f,  0.0f,
// 0.0f,  1.0f,  0.0f
//};

//Cube with Normals
float cubeComplete[] = {
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};