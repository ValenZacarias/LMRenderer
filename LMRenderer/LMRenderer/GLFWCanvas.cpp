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
void GLFWCanvas::SetupContext(VisualizationGroup* viz)
{
	currentViz = viz;
	glEnable(GL_DEPTH_TEST);
}
void GLFWCanvas::Render()
{
	//Rendering config --------------------------------------------------------------------------------------

	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
}

void GLFWCanvas::MousePosHandler(double xpos, double ypos)
{
	if (this == nullptr)
	{
		std::cout << "CANVAS NULL" << std::endl;
		return;
	}

	if(currentTool != nullptr)
		currentTool->OnMouseMove(xpos, ypos);
}

void GLFWCanvas::ScrollHandler(double yoffset)
{
	currentTool->OnScroll(yoffset);
}

void GLFWCanvas::FrameResizeHandler(int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLFWCanvas::KeyboardHandler(GLFWwindow* window)
{
	//ESTO ESTA HACIENDO PULLING NO ES UN EVENTO
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		currentTool->OnKeyPress("W");
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		currentTool->OnKeyPress("S");
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		currentTool->OnKeyPress("A");
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		currentTool->OnKeyPress("D");
		
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		currentTool->OnKeyPress("Q");
		
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		currentTool->OnKeyPress("E");
}
