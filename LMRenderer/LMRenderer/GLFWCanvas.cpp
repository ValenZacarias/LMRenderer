#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "Globals.h"
#include "Tool.h"
#include "ToolFPSCamera.h"
#include "GLFWCanvas.h"
#include "shader_s.h"

GLFWCanvas::GLFWCanvas(int screenWidth, int screenHeigth)
{
	width = screenWidth;
	heigth = screenHeigth;
}

GLFWwindow* GLFWCanvas::Init()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(width, heigth, "Main", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);

	//Callback Binding ------------------------------------------------------------------------

	auto mouseLDragCallback = [](GLFWwindow* w, int button, int action, int mods)
	{
		static_cast<GLFWCanvas*>(glfwGetWindowUserPointer(w))->MouseLDragHandler(button, action, mods);
	};
	glfwSetMouseButtonCallback(window, mouseLDragCallback);

	auto mousePosCallback = [](GLFWwindow* w, double xpos, double ypos)
	{
		static_cast<GLFWCanvas*>(glfwGetWindowUserPointer(w))->MousePosHandler(xpos, ypos);
	};
	glfwSetCursorPosCallback(window, mousePosCallback);

	//This way we can assign methods to glfw callbacks
	auto scrollCallback = [](GLFWwindow* w, double xoffset, double yoffset)
	{
		static_cast<GLFWCanvas*>(glfwGetWindowUserPointer(w))->ScrollHandler(yoffset);
	};
	glfwSetScrollCallback(window, scrollCallback);

	auto frameBufferSizeCallback = [](GLFWwindow* w, int width, int height)
	{
		static_cast<GLFWCanvas*>(glfwGetWindowUserPointer(w))->FrameResizeHandler(width, height);
	};
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, width, heigth);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	

	return window;
}

void GLFWCanvas::SetupContext(VisGroup* viz)
{
	currentViz = viz;
	glEnable(GL_DEPTH_TEST);
}

shared_ptr<Camera> GLFWCanvas::GetCurrentCamara()
{
	return make_shared<Camera>(currentCamera);
}

void GLFWCanvas::UpdateViewMatrix()
{
	currentCamera.view_matrix = glm::lookAt(currentCamera.cameraPos,
		currentCamera.cameraPos + currentCamera.cameraFront,
		currentCamera.cameraUp);
}

void GLFWCanvas::UpdatefrustumPlanes()
{
	//cout << "Camera Front: " << currentCamera.cameraFront.x << " " << currentCamera.cameraFront.y << " " << currentCamera.cameraFront.z << endl;
	if (PERSPECTIVE_CAM)
	{

		currentCamera.halfVSide = currentCamera.perspective_zfar * tanf(currentCamera.FOV * 0.5f);
		currentCamera.halfHSide = currentCamera.halfVSide * (screenWidth / screenHeight);
		currentCamera.frontMultFar = currentCamera.perspective_zfar * currentCamera.cameraFront;

		currentCamera.frustum_leftPlane_normal =  glm::cross(
			currentCamera.frontMultFar - (currentCamera.cameraRight * currentCamera.halfHSide),
			currentCamera.cameraUp);

		currentCamera.frustum_rightPlane_normal = glm::cross(
			currentCamera.cameraUp,
			currentCamera.frontMultFar + (currentCamera.cameraRight * currentCamera.halfHSide));

		currentCamera.frustum_bottomPlane_normal = glm::cross(
			currentCamera.frontMultFar + (currentCamera.cameraUp * currentCamera.halfVSide),
			currentCamera.cameraRight);

		currentCamera.frustum_topPlane_normal = glm::cross(
			currentCamera.cameraRight,
			currentCamera.frontMultFar - (currentCamera.cameraUp * currentCamera.halfVSide));

		//currentCamera.frustum_leftPlane_distance	= glm::dot(currentCamera.cameraPos, currentCamera.frustum_leftPlane_normal);
		//currentCamera.frustum_rightPlane_distance	= glm::dot(currentCamera.cameraPos, currentCamera.frustum_rightPlane_normal);
		//currentCamera.frustum_bottomPlane_distance	= glm::dot(currentCamera.cameraPos, currentCamera.frustum_bottomPlane_normal);
		//currentCamera.frustum_topPlane_distance		= glm::dot(currentCamera.cameraPos, currentCamera.frustum_topPlane_normal);
		
		// Funciona solamente en z- y x-
		currentCamera.frustum_leftPlane_distance	= glm::distance(currentCamera.cameraPos, glm::vec3(0.0f));
		currentCamera.frustum_rightPlane_distance	= glm::distance(currentCamera.cameraPos, glm::vec3(0.0f));
		currentCamera.frustum_bottomPlane_distance	= glm::distance(currentCamera.cameraPos, glm::vec3(0.0f));
		currentCamera.frustum_topPlane_distance		= glm::distance(currentCamera.cameraPos, glm::vec3(0.0f));
	}
}

void GLFWCanvas::Updatefrustum()
{

	// ORTHOGONAL
	currentCamera.ortho_frustum[0] = -2.0f * currentCamera.FOV * 0.05f;
	currentCamera.ortho_frustum[1] = +2.0f * currentCamera.FOV * 0.05f;
	currentCamera.ortho_frustum[2] = -1.15f * currentCamera.FOV * 0.05f;
	currentCamera.ortho_frustum[3] = +1.15f * currentCamera.FOV * 0.05f;
	currentCamera.ortho_frustum[4] = currentCamera.ortho_znear;
	currentCamera.ortho_frustum[5] = currentCamera.ortho_zfar;

	currentCamera.ortho_matrix = glm::ortho(currentCamera.ortho_frustum[0],
											currentCamera.ortho_frustum[1],
											currentCamera.ortho_frustum[2],
											currentCamera.ortho_frustum[3],
											currentCamera.ortho_frustum[4],
											currentCamera.ortho_frustum[5]);

	// PERSPECTIVE
	currentCamera.perspective_matrix = glm::perspective(glm::radians(currentCamera.FOV), 
														(float)screenWidth / (float)screenHeight, 
														currentCamera.perspective_znear, 
														currentCamera.perspective_zfar);
	
	UpdatefrustumPlanes();

}

void GLFWCanvas::Render()
{
	//Rendering config --------------------------------------------------------------------------------------

	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	UpdatefrustumPlanes();

	currentViz->Render(&currentCamera);

}

void GLFWCanvas::SetCurrentTool(Tool* tool)
{
	if (tool != nullptr)
		currentTool = tool;
}

void GLFWCanvas::MouseLDragHandler(int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		currentTool->OnDrag(PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		currentTool->OnDrag(RELEASE);
	}
	UpdateViewMatrix();
}

void GLFWCanvas::MousePosHandler(double xpos, double ypos)
{

	if(currentTool != nullptr)
		currentTool->OnMouseMove(xpos, ypos);

	UpdateViewMatrix();
	UpdatefrustumPlanes();
}

void GLFWCanvas::ScrollHandler(double yoffset)
{
	currentTool->OnScroll(yoffset);
	Updatefrustum();
	UpdatefrustumPlanes();
}

void GLFWCanvas::FrameResizeHandler(int width, int height)
{
	glViewport(0, 0, width, height);
	UpdatefrustumPlanes();
}

void GLFWCanvas::KeyboardHandler(GLFWwindow* window)
{
	//ESTO ESTA HACIENDO PULLING NO ES UN EVENTO
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{ 
		currentTool->OnKeyPress("W");
		Updatefrustum();
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		currentTool->OnKeyPress("S");
		Updatefrustum();
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		currentTool->OnKeyPress("A");
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		currentTool->OnKeyPress("D");
		
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		currentTool->OnKeyPress("Q");
		
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		currentTool->OnKeyPress("E");

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		CAMERA_SPEED += 0.5;
		cout << "CAM SPEED = " << CAMERA_SPEED << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		CAMERA_SPEED -= 0.5;
		cout << "CAM SPEED = " << CAMERA_SPEED << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		currentCamera.cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
			PERSPECTIVE_CAM = true;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		PERSPECTIVE_CAM = false;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		currentCamera.ortho_znear += 0.1f;
		Updatefrustum();
		//cout << "CAMERA ZFAR = " << currentCamera.ortho_zfar << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		currentCamera.ortho_znear -= 0.1f;
		Updatefrustum();
		//cout << "CAMERA ZFAR = " << currentCamera.ortho_zfar << endl;
	}

	UpdateViewMatrix();
	Updatefrustum();
}
