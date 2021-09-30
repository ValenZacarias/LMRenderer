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

#include "VisBase.h"
#include "VisGroup.h"
#include "VisGrid.h"
#include "VisPrimitive.h"
#include "VisIndxElement.h"

#include "DataStructureBase.h"
#include "DataVector.h"

#include "PATriangulate.h"

#include "PMeshParser.h"



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
	ToolFPSCamera cameraTool = ToolFPSCamera(&canvas.currentCamera);
	//ToolOrbitCamera cameraTool = ToolOrbitCamera(&canvas.currentCamera);
	canvas.SetCurrentTool(&cameraTool);
	
	//PARSING AND DATA GENERATION -------------------------------------------------------------------------------------------
	PMeshParser Parser = PMeshParser();
	//auto DataIndex = make_shared<vector<DataVector<int>>>(Parser.ParseFaces("testmesh_45e_faces.txt"));
	//shared_ptr<DataVector<float>> DataVertex = Parser.ParsePoints("testmesh_45e_points.txt");
	
	auto DataIndex = Parser.ParseFaces("Meshes/testmesh_45e_faces.txt");
	auto DataVertex = Parser.ParsePoints("Meshes/testmesh_45e_points.txt");	
	auto DataOwner = Parser.ParseCells("Meshes/testmesh_45e_owner.txt");
	auto DataNeighbour= Parser.ParseCells("Meshes/testmesh_45e_neighbour.txt");

	//auto DataIndex = Parser.ParseFaces("Meshes/flange_mf_282k_faces.txt");
	//auto DataVertex = Parser.ParsePoints("Meshes/flange_mf_282k_points.txt");

	//VISUALIZATIONS-----------------------------------------------------------------------------------------------------------
	VisIndxElement<shared_ptr<DataVector<float>>, shared_ptr<vector<DataVector<int>>>> vizElement(DataVertex, DataIndex);
	VisGrid vizGrid = VisGrid();

	VisGroup MainGroupViz = VisGroup();
	MainGroupViz.visualizations.push_back(&vizElement);
	MainGroupViz.visualizations.push_back(&vizGrid);
	canvas.SetupContext(&MainGroupViz);

	float currentFrame = 0.0f;
	std::stringstream ss;
	int nbFrames = 0;
	double fps;
	char title_string[11] = "LMRenderer";
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

		// FPS Counter
		if (deltaTime > 0.015)
		{
			fps = round((double)nbFrames / deltaTime);
			sprintf_s(title_string, "FPS: %.1f", fps);
			glfwSetWindowTitle(window, title_string);
			lastFrame = currentFrame;
			nbFrames = 0;
		}

		nbFrames++;
	}

	glfwTerminate();
	return 0;
}

