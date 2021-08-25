#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "Globals.h"
#include "Tool.h"
#include "ToolOrbitCamera.h"

void ToolOrbitCamera::OnScroll(double yoffset)
{
	cam->FOV -= yoffset;
}

void ToolOrbitCamera::OnDrag(MouseState state)
{
	this->actualState = state;
}

void ToolOrbitCamera::OnMouseMove(double xpos, double ypos)
{
	

	if (actualState == PRESS)
	{
		xAngle = (cam->lastX - xpos)* 2.0f * deltaAngleX;
		yAngle = (cam->lastY - ypos)* 2.0f * deltaAngleY;

		rotationMatrixX = glm::rotate(rotationMatrixX, glm::radians(xAngle), -cam->cameraUp);
		cam->cameraPos = (rotationMatrixX * (glm::vec4(cam->cameraPos, 1.0f) - pivot)) + pivot;
		
		rotationMatrixY = glm::rotate(rotationMatrixY, glm::radians(yAngle), cam->cameraRight);
		cam->cameraPos = (rotationMatrixY * (glm::vec4(cam->cameraPos, 1.0f) - pivot)) + pivot;

		cam->cameraFront = glm::normalize(-cam->cameraPos); // CAMBIAR POR LOOKAT
		
		std::cout << "ANG X " << xAngle << std::endl;
		std::cout << "ANG Y " << yAngle << std::endl;
		
		cam->lastX = xpos;
		cam->lastY = ypos;
	}
	if (actualState == RELEASE)
	{
		xAngle = 0;
		yAngle = 0;
		rotationMatrixX = glm::mat4(1.0f);
		rotationMatrixY = glm::mat4(1.0f);
		cam->lastX = xpos;
		cam->lastY = ypos;
		return;
	}
}

void ToolOrbitCamera::OnKeyPress(std::string key)
{
	if (key == "W")
		cam->cameraPos += cam->cameraSpeed * deltaTime * cam->cameraFront;

	if (key == "S")
		cam->cameraPos -= cam->cameraSpeed * deltaTime * cam->cameraFront;
}