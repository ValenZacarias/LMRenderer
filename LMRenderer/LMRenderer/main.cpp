#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "Globals.h"

#include "Tool.h"
#include "ToolFPSCamera.h"
#include "GLFWCanvas.h"

#include "VisualizationBase.h"
#include "VisualizationGroup.h"
#include "VisualizationPrimitive.h"

#include "DataStructureBase.h"
#include "DataVectorTest.h"

#include "main.h"

int screenWidth = 1280;
int screenHeigth = 720;

float lastFrame = 0.0f;

GLFWwindow* window = NULL;

float cubeVertices[] =
{
-0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
-0.5f,  0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,

-0.5f, -0.5f,  0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,
-0.5f, -0.5f,  0.5f,

-0.5f,  0.5f,  0.5f,
-0.5f,  0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,
-0.5f, -0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,

 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,

-0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f, -0.5f,  0.5f,
-0.5f, -0.5f,  0.5f,
-0.5f, -0.5f, -0.5f,

-0.5f,  0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,
-0.5f,  0.5f, -0.5f,
};

float cubeNormals[] =
{
 0.0f,  0.0f, -1.0f,
 0.0f,  0.0f, -1.0f,
 0.0f,  0.0f, -1.0f,
 0.0f,  0.0f, -1.0f,
 0.0f,  0.0f, -1.0f,
 0.0f,  0.0f, -1.0f,

 0.0f,  0.0f,  1.0f,
 0.0f,  0.0f,  1.0f,
 0.0f,  0.0f,  1.0f,
 0.0f,  0.0f,  1.0f,
 0.0f,  0.0f,  1.0f,
 0.0f,  0.0f,  1.0f,

-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,

 1.0f,  0.0f,  0.0f,
 1.0f,  0.0f,  0.0f,
 1.0f,  0.0f,  0.0f,
 1.0f,  0.0f,  0.0f,
 1.0f,  0.0f,  0.0f,
 1.0f,  0.0f,  0.0f,

 0.0f, -1.0f,  0.0f,
 0.0f, -1.0f,  0.0f,
 0.0f, -1.0f,  0.0f,
 0.0f, -1.0f,  0.0f,
 0.0f, -1.0f,  0.0f,
 0.0f, -1.0f,  0.0f,

 0.0f,  1.0f,  0.0f,
 0.0f,  1.0f,  0.0f,
 0.0f,  1.0f,  0.0f,
 0.0f,  1.0f,  0.0f,
 0.0f,  1.0f,  0.0f,
 0.0f,  1.0f,  0.0f
};

float pyramidVertices[] = 
{
	// first triangle texture
	0.5f, -0.5f, -0.5f, // upper right corner
	0.5f, -0.5f, 0.5f,  // lower right corner
	-0.5f, -0.5f, -0.5f, // upper left corner
	// second triangle
	0.5f, -0.5f, 0.5f, // lower right corner
	-0.5f, -0.5f, 0.5f,// lower left corner
	-0.5f, -0.5f, -0.5f // upper left corner
	//The third triangl
   -0.5f, -0.5f, 0.5f, // lower left corner
	0.0f, 0.4f, 0.0f,  // vertex
	0.5f, -0.5f, 0.5f, // lower right corner
	//fourth triangle
	0.5f, -0.5f, 0.5f, // lower right corner
	0.0f, 0.4f, 0.0f,  // vertex
	0.5f, -0.5f, -0.5f,// upper right corner
	//The fifth triangl
	0.5f, -0.5f, -0.5f,// upper right corner
	0.0f, 0.4f, 0.0f,   // vertex
	-0.5f, -0.5f, -0.5f // upper left corner
	// sixth triangle
	-0.5f, -0.5f, -0.5f, // upper left corner
	0.0f, 0.4f, 0.0f,   // vertex
	-0.5f, -0.5f, 0.5f, // lower left corner
};

int main()
{
	//CANVAS SETUP ----------------------------------------------------------------------------------------------------------
	GLFWCanvas canvas = GLFWCanvas(1280, 720);
	window = canvas.Init();

	//Ahora le damos un ptr a algo que esta en el stack y muere con el main
	//Si la idea es ir creando y destruyendo a medida que cambiamos, podemos usar smart pointers
	ToolFPSCamera cameraTool = ToolFPSCamera(&canvas.currentCamera);
	canvas.SetCurrentTool(&cameraTool);
	
	glEnable(GL_DEPTH_TEST);
	Shader lightShader("light_vertex_shader.txt", "light_fragment_shader.txt");

	//DATA -----------------------------------------------------------------------------------------------------------------
	int count = sizeof(cubeVertices) / sizeof(*cubeVertices);
	std::shared_ptr<DataVectorTest<float>> DataVertex = std::make_shared<DataVectorTest<float>>(FLOATVAL, cubeVertices, count);
	
	count = sizeof(cubeNormals) / sizeof(*cubeNormals);
	std::shared_ptr<DataVectorTest<float>> DataNormal = std::make_shared<DataVectorTest<float>>(FLOATVAL, cubeNormals, count);
	
	//VISUALIZATION SETUP --------------------------------------------------------------------------------------------------
	VisualizationPrimitive testViz = VisualizationPrimitive(DataVertex, DataNormal);

	//TODO: smart pointers
	VisualizationGroup MainGroupViz = VisualizationGroup();
	MainGroupViz.visualizations.push_back(&testViz);
	canvas.SetupContext(&MainGroupViz);

	float currentFrame = 0.0f;
	//RENDER LOOP! -----------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		canvas.KeyboardHandler(window);
		canvas.Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//TODO
	//Pasar todos los glDelete a los destructores de Data, visualization, etc

	// de-allocate all resources once they've outlived their purpose:
	//glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteBuffers(1, &cubeVBO);
	//glDeleteVertexArrays(1, &pyramidVAO);
	//glDeleteBuffers(1, &pyramidVBO);
	//glDeleteVertexArrays(1, &lightVAO);

	glfwTerminate();
	return 0;
}


