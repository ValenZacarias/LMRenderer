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

	// MATRIX
	glm::mat4 view_matrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	float ortho_zfar_fixed = 100;
	float ortho_znear_fixed = -100;

	float ortho_zfar = 100;
	float ortho_znear = -100;

	float perspective_zfar = 5000.0f;
	float perspective_znear = 0.01f;

	vector<float> ortho_frustum = { -2.0f * FOV * 0.05f,	// left
									+2.0f * FOV * 0.05f,	// right
									-1.15f * FOV * 0.05f,	// bottom
									+1.15f * FOV * 0.05f,	// top
									ortho_znear,				// znear
									ortho_zfar};				// zfar

	glm::mat4 ortho_matrix = glm::ortho(ortho_frustum[0],
										ortho_frustum[1],
										ortho_frustum[2],
										ortho_frustum[3],
										ortho_frustum[4],
										ortho_frustum[5]);

	glm::mat4 perspective_matrix = glm::perspective(glm::radians(FOV), (float)screenWidth / (float)screenHeight, perspective_znear, perspective_zfar);

	// frustum
	float halfVSide = 0.0f;
	float halfHSide = 0.0f;
	glm::vec3 frontMultFar = perspective_zfar * cameraFront;

	glm::vec3 frustum_leftPlane_normal = glm::vec3(0.0f);
	glm::vec3 frustum_rightPlane_normal = glm::vec3(0.0f);
	glm::vec3 frustum_bottomPlane_normal = glm::vec3(0.0f);
	glm::vec3 frustum_topPlane_normal = glm::vec3(0.0f);

	float frustum_leftPlane_distance = 0.0f;
	float frustum_rightPlane_distance = 0.0f;
	float frustum_bottomPlane_distance = 0.0f;
	float frustum_topPlane_distance = 0.0f;

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
	void UpdateViewMatrix();
	void UpdatefrustumPlanes();
	void Updatefrustum();
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

