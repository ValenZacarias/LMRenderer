#pragma once
#include <glad/glad.h>

#include <string>
#include <iostream>
#include <glm/glm.hpp> 

#include "Tool.h"
#include "GLFWCanvas.h"

struct Camera;

class ToolFPSCamera : public Tool
{

public:
	Camera* cam;

	ToolFPSCamera(Camera* currentCamera)
	{
		std::cout << "ToolFPSCamera instance created" << std::endl;
		cam = currentCamera;
	}

	void OnScroll(double yoffset) override;
	void OnDrag(MouseState state) override;
	void OnMouseMove(double xpos, double ypos) override;
	void OnKeyPress(std::string key) override;
};