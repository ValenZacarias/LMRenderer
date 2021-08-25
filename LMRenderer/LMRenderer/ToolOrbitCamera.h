#pragma once
#include <glad/glad.h>

#include <string>
#include <iostream>
#include <glm/glm.hpp> 

#include "Tool.h"
#include "Globals.h"
#include "GLFWCanvas.h"

# define M_PI           3.14159265358979323846  /* pi */

struct Camera;

class ToolOrbitCamera : public Tool
{
private:
	MouseState actualState = NONE;
	glm::vec4 pivot = glm::vec4(0.0f);

	glm::mat4x4 rotationMatrixX = glm::mat4x4(1.0f);
	glm::mat4x4 rotationMatrixY = glm::mat4x4(1.0f);

	float deltaAngleX = (2 * M_PI / screenWidth);
	float deltaAngleY = (M_PI / screenHeight);   
	float xAngle;
	float yAngle;
public:
	Camera* cam;

	ToolOrbitCamera(Camera* currentCamera)
	{
		std::cout << "ToolOrbitCamera instance created" << std::endl;
		cam = currentCamera;
		cam->cameraFront = glm::normalize(-cam->cameraPos);;
	}

	void OnScroll(double yoffset) override;
	void OnDrag(MouseState state) override;
	void OnMouseMove(double xpos, double ypos) override;
	void OnKeyPress(std::string key) override;
};