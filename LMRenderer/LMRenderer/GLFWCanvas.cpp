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
	Shader diffuseShader("difusse_vertex_shader.txt", "difusse_fragment_shader.txt");
	//diffuseShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//diffuseShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
	
	currentViz = viz;
	currentShader = diffuseShader;
}
void GLFWCanvas::Render()
{
	//Rendering config --------------------------------------------------------------------------------------
	glClearColor(0.05f, 0.28f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

	//3D Transformations --------------------------------------------------------------------------------------
	//view: World -> View
	//glm::mat4 view;
	//view = glm::lookAt(currentCamera.cameraPos, currentCamera.cameraPos + currentCamera.cameraFront, currentCamera.cameraUp);
	//unsigned int viewLoc = glGetUniformLocation(currentShader.ID, "view");
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	////projection: View -> Clip
	//glm::mat4 projection;
	//projection = glm::perspective(glm::radians(currentCamera.FOV), (float)width / (float)heigth, 1.0f, 1000.0f);
	//unsigned int projectionLoc = glGetUniformLocation(currentShader.ID, "projection");
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//std::cout << "CurrentShad ID: " << currentShader.ID << std::endl;

	currentViz->Render(&currentCamera, &currentShader);
}

void GLFWCanvas::SetCurrentTool(Tool* tool)
{
	if (tool != nullptr)
		currentTool = tool;
}

void GLFWCanvas::MousePosHandler(double xpos, double ypos)
{
	//std::cout << "MP hand:" << xpos << " - " << ypos << std::endl;
	//currentTool.OnMouseMove(xpos, ypos);
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
	//currentTool.OnScroll(yoffset);
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
	//cameraPos += cameraSpeed * deltaTime * cameraFront;
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		currentTool->OnKeyPress("S");
	//cameraPos -= cameraSpeed * deltaTime * cameraFront;
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		currentTool->OnKeyPress("A");
	//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		currentTool->OnKeyPress("D");
	//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
		
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		currentTool->OnKeyPress("Q");
	//cameraPos -= cameraUp * cameraSpeed * deltaTime;
		
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		currentTool->OnKeyPress("E");
	//cameraPos += cameraUp * cameraSpeed * deltaTime;
}