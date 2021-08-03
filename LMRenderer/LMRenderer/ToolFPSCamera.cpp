#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "Globals.h"
#include "Tool.h"
#include "ToolFPSCamera.h"

void ToolFPSCamera::OnScroll(double yoffset)
{
	std::cout << cam->FOV << std::endl;
	cam->FOV -= yoffset;
}

void ToolFPSCamera::OnMouseMove(double xpos, double ypos)
{
	if (firstMouse) // initially set to true 
	{
		cam->lastX = xpos;
		cam->lastY = ypos;
		firstMouse = false;
	}
	cam->xoffset = xpos - cam->lastX;
	cam->yoffset = cam->lastY - ypos;

	cam->lastX = xpos;
	cam->lastY = ypos;

	cam->yoffset *= cam->sensitivity;
	cam->xoffset *= cam->sensitivity;

	cam->yaw += cam->xoffset;
	cam->pitch += cam->yoffset;

	cam->front.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
	cam->front.y = sin(glm::radians(cam->pitch));
	cam->front.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));

	cam->cameraFront = glm::normalize(cam->front);
}

void ToolFPSCamera::OnKeyPress(std::string key)
{
	if (key == "W")
		cam->cameraPos += cam->cameraSpeed * deltaTime * cam->cameraFront;
	//cameraPos += cameraSpeed * cameraFront;

	if (key == "S")
		cam->cameraPos -= cam->cameraSpeed * deltaTime * cam->cameraFront;
	//cameraPos -= cameraSpeed * cameraFront;

	if (key == "A")
		cam->cameraPos -= glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * cam->cameraSpeed * deltaTime;
	//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (key == "D")
		cam->cameraPos += glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * cam->cameraSpeed * deltaTime;
	//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (key == "Q")
		cam->cameraPos -= cam->cameraUp * cam->cameraSpeed * deltaTime;
	//cameraPos -= cameraUp * cameraSpeed;

	if (key == "E")
		cam->cameraPos += cam->cameraUp * cam->cameraSpeed * deltaTime;
	//cameraPos += cameraUp * cameraSpeed;

}