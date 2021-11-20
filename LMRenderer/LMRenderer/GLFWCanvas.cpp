#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Globals.h"
#include "Tool.h"
#include "ToolFPSCamera.h"
#include "GLFWCanvas.h"
#include "shader_s.h"

//#include "DynamicLoadingManager.h"



//#include "VisMeshZone.h"

GLFWCanvas::GLFWCanvas(int screenWidth, int screenHeigth)
{
	width = screenWidth;
	heigth = screenHeigth;
}
GLFWCanvas::~GLFWCanvas()
{
	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

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
	glm::vec3 camRight = glm::normalize(glm::cross(currentCamera.cameraFront, currentCamera.cameraUp));
	glm::vec3 camUp = glm::normalize(glm::cross(currentCamera.cameraFront, camRight));
	//float scaleCorrection = 0.8f;

	float halfVSide = currentCamera.perspective_zfar * tanf(glm::radians(currentCamera.FOV) * 0.5f) * FRUSTRUM_SCALE;
	float halfHSide = halfVSide * (screenWidth / screenHeight) * FRUSTRUM_SCALE * 1.25;
	glm::vec3 frontMultFar = currentCamera.perspective_zfar * currentCamera.cameraFront;

	currentCamera.frustum_leftPlane_normal = - glm::cross( camUp, frontMultFar - (camRight * halfVSide));

	currentCamera.frustum_rightPlane_normal = - glm::cross( frontMultFar + (camRight * halfVSide), camUp);

	currentCamera.frustum_bottomPlane_normal = glm::cross( frontMultFar + (camUp * halfHSide), camRight);

	currentCamera.frustum_topPlane_normal = glm::cross( camRight, frontMultFar - (camUp * halfHSide));

}

void GLFWCanvas::UpdatefrustumMatrix()
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
}


void GLFWCanvas::UIRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("Guia de comandos");
	//ImGui::SetWindowFontScale(1.15);
	ImGui::Text("1. Mover camara - W, A, S, D, Q, E\n"
				"2. Resetear la posicion de la camara - R\n"
				"3. Camara Perspectiva - 1\n"
				"4. Camara Isometrica - 2\n"
				"5. Velocidad de camara -/+ - K, L\n"
				"6. Activar/Desactivar Cursor - P, O\n"
				"");

	ImGui::End();

	ImGui::Begin("Opciones");
	//ImGui::SetWindowFontScale(1.15);
	ImGui::SliderFloat("Frustrum Scale", &FRUSTRUM_SCALE, 0.01f, 2.0f);
	ImGui::SliderFloat("Camera Speed", &CAMERA_SPEED, 0.0f, 10.0f);
	ImGui::SliderFloat("Mouse Speed", &MOUSE_SENSITIVITY, 0.0f, 0.5f);
	ImGui::End();

	// UI para saber estado de las zonas
	ImGui::Begin("Zone state");
	//ImGui::SetWindowFontScale(1.15);
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < currentViz->shared_visualizations.size(); n++)
	{
		if (currentViz->shared_visualizations[n]->GetCurrentLevel() != -2)
		{
			bool x = currentViz->shared_visualizations[n]->IsVisible();
			
			ImGui::Bullet(); ImGui::SameLine();
			ImGui::Text("Zona %.1i -", n); ImGui::SameLine();
			ImGui::Checkbox("", &x); ImGui::SameLine();
			ImGui::Text(" Lv: %.1i", currentViz->shared_visualizations[n]->GetCurrentLevel());
		}
	}
	ImGui::EndChild();
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLFWCanvas::Render()
{
	//Rendering config --------------------------------------------------------------------------------------
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	currentViz->Render(&currentCamera);
	UIRender();
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
	UpdatefrustumPlanes();
	UpdatefrustumMatrix();
	UpdateViewMatrix();
}

void GLFWCanvas::MousePosHandler(double xpos, double ypos)
{
	if (CAM_ENABLED)
	{
		if (currentTool != nullptr)
			currentTool->OnMouseMove(xpos, ypos);

		UpdatefrustumPlanes();
		UpdatefrustumMatrix();
		UpdateViewMatrix();
	}
	
}

void GLFWCanvas::ScrollHandler(double yoffset)
{
	currentTool->OnScroll(yoffset);
	UpdatefrustumPlanes();
	UpdatefrustumMatrix();
	UpdateViewMatrix();
}

void GLFWCanvas::FrameResizeHandler(int width, int height)
{
	glViewport(0, 0, width, height);
	UpdatefrustumPlanes();
	UpdatefrustumMatrix();
	UpdateViewMatrix();
}

void GLFWCanvas::KeyboardHandler(GLFWwindow* window)
{
	//ESTO ESTA HACIENDO PULLING NO ES UN EVENTO
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{ 
		currentTool->OnKeyPress("W");
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		currentTool->OnKeyPress("S");
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
		CAMERA_SPEED += 0.25;
		cout << "CAM SPEED = " << CAMERA_SPEED << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		if (CAMERA_SPEED < 0.1)
			return;

		CAMERA_SPEED -= 0.25;
		cout << "CAM SPEED = " << CAMERA_SPEED << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		currentCamera.cameraPos = currentCamera.cameraRestartPos;
		currentCamera.pitch = currentCamera.restartPitch;
		currentCamera.yaw = currentCamera.restartYaw;
		currentCamera.xoffset = 0.0f;
		currentCamera.yoffset = 0.0f;
		currentTool->OnMouseMove(0, 0);
		firstMouse = true;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
			PERSPECTIVE_CAM = true;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		PERSPECTIVE_CAM = false;
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		CAM_ENABLED = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		CAM_ENABLED = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	UpdatefrustumPlanes();
	UpdatefrustumMatrix();
	UpdateViewMatrix();
}
