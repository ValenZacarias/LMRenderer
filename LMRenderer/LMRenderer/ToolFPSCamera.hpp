#pragma once
#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp> 

#include "Tool.hpp"
#include "GLFWCanvas.hpp" //Para acceder a camera debo incluir este HPP, esta bien?


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
	void OnMouseMove(double xpos, double ypos) override;
	void OnKeyPress(std::string key) override;
};