#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "Globals.h"
#include "Tool.h"
#include "ToolFPSCamera.h"
#include "shader_s.h"

#include "VisGroup.h"

struct Camera
{
	//Camera settings
	const float sensitivity = 0.1f;
	const float cameraSpeed = CAMERA_SPEED;
	float FOV = 45.0f;
	//float FOV = 5.0f; //ortho close
	//float FOV = 55.0f; //ortho far

	//glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 2.0f);
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.02f, 0.15f);
	glm::vec3 cameraFront = glm::vec3(0.0f, -0.5f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f);
	glm::vec3 front = glm::vec3(0.0f);

	float ortho_zfar_fixed = 100;
	float ortho_znear_fixed = -100;

	float ortho_zfar = 100;
	float ortho_znear = -100;

	vector<float> ortho_frustrum = { -2.0f * FOV * 0.05f,	// left
									+2.0f * FOV * 0.05f,	// right
									-1.15f * FOV * 0.05f,	// bottom
									+1.15f * FOV * 0.05f,	// top
									ortho_znear,				// znear
									ortho_zfar};				// zfar

	//glm::mat4 projection = glm::perspective(glm::radians(FOV), 1280.0f / 720.0f, 0.01f, 5000.0f);

	//Camera Pos and Rot settings
	float pitch = 0.0f;
	float yaw = 0.0f;
	float lastX = 400.0f;
	float lastY = 300.0f;
	float xoffset = 0.0f;
	float yoffset = 0.0f;
};

class GLFWCanvas
{
private:
	int width;
	int heigth;
	Tool* currentTool; //Tool deberia ser un smartpointer a tool
	VisGroup* currentViz; 
	Shader currentShader;
	glm::vec3 lightPos = glm::vec3(0.0f, 8.0f, 0.0f);
	glm::vec3 backgroundColor = glm::vec3(0.02f, 0.08f, 0.09f);
	

public:
	Camera currentCamera;

	GLFWCanvas(int screenWidth, int screenHeigth);
	~GLFWCanvas() {};
	void SetupContext(VisGroup* viz);
	void UpdateFrustrum();
	void Render();
	void SetCurrentTool(Tool* tool);
	void MouseLDragHandler(int button, int action, int mods);
	void MousePosHandler(double xpos, double ypos);
	void ScrollHandler(double yoffset);
	void FrameResizeHandler(int width, int height);
	void KeyboardHandler(GLFWwindow* window);
	shared_ptr<Camera> GetCurrentCamara();

	GLFWwindow* Init();
};

