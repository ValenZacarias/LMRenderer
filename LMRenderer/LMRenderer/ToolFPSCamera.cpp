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

	cam->yoffset *= MOUSE_SENSITIVITY;
	cam->xoffset *= MOUSE_SENSITIVITY;

	cam->yaw += cam->xoffset;
	cam->pitch += cam->yoffset;

	// Calcular right y front aca, no hacerlos atributos de camera

	float front_x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
	float front_y = sin(glm::radians(cam->pitch));
	float front_z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));

	cam->cameraFront = glm::normalize(glm::vec3(front_x, front_y, front_z));
	cam->cameraRight = glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp));
	cam->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
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
		cam->cameraPos -= cam->cameraRight * CAMERA_SPEED * deltaTime;
		//cam->cameraPos -= glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * CAMERA_SPEED * deltaTime;

	if (key == "D")
		cam->cameraPos += cam->cameraRight * CAMERA_SPEED * deltaTime;
		//cam->cameraPos += glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * CAMERA_SPEED * deltaTime;

	if (key == "Q")
		cam->cameraPos -= cam->cameraUp * CAMERA_SPEED * deltaTime;

	if (key == "E")
		cam->cameraPos += cam->cameraUp * CAMERA_SPEED * deltaTime;
}