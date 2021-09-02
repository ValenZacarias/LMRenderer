#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "Globals.h"

#include "Tool.h"
#include "ToolFPSCamera.h"
#include "ToolOrbitCamera.h"

#include "GLFWCanvas.h"

#include "VisualizationBase.h"
#include "VisualizationGroup.h"
#include "VisualizationGrid.h"
#include "VisualizationPrimitive.h"
#include "VisualizationIndxElement.h"

#include "DataStructureBase.h"
#include "DataVector.h"

#include "PATriangulate.h"

#include "PMeshParserTxt.h"



#include "main.h"

float lastFrame = 0.0f;

GLFWwindow* window = NULL;


int main()
{
	//CANVAS SETUP ----------------------------------------------------------------------------------------------------------
	GLFWCanvas canvas = GLFWCanvas(screenWidth, screenHeight);
	window = canvas.Init();

	//Ahora le damos un ptr a algo que esta en el stack y muere con el main
	//Si la idea es ir creando y destruyendo a medida que cambiamos, podemos usar smart pointers
	//ToolFPSCamera cameraTool = ToolFPSCamera(&canvas.currentCamera);
	ToolOrbitCamera cameraTool = ToolOrbitCamera(&canvas.currentCamera);
	canvas.SetCurrentTool(&cameraTool);
	
	//PARSING AND DATA GENERATION -------------------------------------------------------------------------------------------
	PMeshPareserTxt Parser = PMeshPareserTxt();
	auto DataIndex = make_shared<vector<DataVector<int>>>(Parser.ParseFaces("testmesh_45e_faces.txt"));
	shared_ptr<DataVector<float>> DataVertex = Parser.ParsePoints("testmesh_45e_points.txt");


	//VISUALIZATIONS-----------------------------------------------------------------------------------------------------------
	VisualizationIndxElement<shared_ptr<DataVector<float>>, shared_ptr<vector<DataVector<int>>>> vizElement(DataVertex, DataIndex);
	VisualizationGrid vizGrid = VisualizationGrid();

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


