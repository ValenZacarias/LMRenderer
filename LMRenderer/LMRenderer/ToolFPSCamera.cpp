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
	cam->FOV -= yoffset;
}

void ToolFPSCamera::OnDrag(MouseState state)
{
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

	cam->yoffset *= cam->sensitivity * (CAMERA_SPEED * 0.1);
	cam->xoffset *= cam->sensitivity * (CAMERA_SPEED * 0.1);

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
	{
		if (PERSPECTIVE_CAM)
			cam->cameraPos += CAMERA_SPEED * deltaTime * cam->cameraFront;
		else
			cam->FOV -= 0.1 * CAMERA_SPEED;

	}

	if (key == "S")
	{
		if(PERSPECTIVE_CAM)
			cam->cameraPos -= CAMERA_SPEED * deltaTime * cam->cameraFront;
		else
			cam->FOV += 0.1 * CAMERA_SPEED;
	}

	if (key == "A")
		cam->cameraPos -= glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * CAMERA_SPEED * deltaTime;

	if (key == "D")
		cam->cameraPos += glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * CAMERA_SPEED * deltaTime;

	if (key == "Q")
		cam->cameraPos -= cam->cameraUp * CAMERA_SPEED * deltaTime;

	if (key == "E")
		cam->cameraPos += cam->cameraUp * CAMERA_SPEED * deltaTime;
}