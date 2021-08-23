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
#include "VisualizationGrid.h"
#include "VisualizationPrimitive.h"
#include "VisualizationIndxElement.h"

#include "DataStructureBase.h"
#include "DataVectorTest.h"

#include "PATriangulate.h"

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

float cubeElemVertices[]
{
	// front
	-0.5, -0.5,  0.5,
	 0.5, -0.5,  0.5,
	 0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,
	// back
	-0.5, -0.5, -0.5,
	 0.5, -0.5, -0.5,
	 0.5,  0.5, -0.5,
	-0.5,  0.5, -0.5
};

float cubeElemIndex[]
{
	0, 1, 2, 3,
	1, 5, 6, 2,
	7, 6, 5 ,4,
	4, 0, 3, 7,
	4, 5, 1, 0,
	3, 2, 6, 7

};


int main()
{
	//CANVAS SETUP ----------------------------------------------------------------------------------------------------------
	GLFWCanvas canvas = GLFWCanvas(screenWidth, screenHeigth);
	window = canvas.Init();

	//Ahora le damos un ptr a algo que esta en el stack y muere con el main
	//Si la idea es ir creando y destruyendo a medida que cambiamos, podemos usar smart pointers
	ToolFPSCamera cameraTool = ToolFPSCamera(&canvas.currentCamera);
	canvas.SetCurrentTool(&cameraTool);
	
	

	//DATA -----------------------------------------------------------------------------------------------------------------
	int count = sizeof(cubeElemVertices) / sizeof(*cubeElemVertices);

	auto DataVertex = std::make_shared<DataVectorTest<float>>(FLOATVAL);
	DataVertex->ReserveData(count);
	for (int i = 0; i < count; i++) { DataVertex->SetData(cubeElemVertices[i]);}
	
	auto DataIndex = std::make_shared<DataVectorTest<int>>(INTVAL);
	DataIndex->ReserveData(count);
	for (int i = 0; i < count; i++) { DataIndex->SetData(cubeElemIndex[i]); }
	
	//VISUALIZATION SETUP --------------------------------------------------------------------------------------------------
	//VisualizationPrimitive testViz = VisualizationPrimitive(DataVertex, DataNormal);
	VisualizationIndxElement vizElement = VisualizationIndxElement(DataVertex, DataIndex);
	VisualizationGrid vizGrid = VisualizationGrid();

	//TODO: smart pointers
	VisualizationGroup MainGroupViz = VisualizationGroup();
	MainGroupViz.visualizations.push_back(&vizElement);
	MainGroupViz.visualizations.push_back(&vizGrid);
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

	glfwTerminate();
	return 0;
}


