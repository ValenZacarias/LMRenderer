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

	//auto DataIndex = Parser.ParseFaces("Meshes/testmesh_45e_faces.txt");
	//auto DataVertex = Parser.ParsePoints("Meshes/testmesh_45e_points.txt");	
	//auto DataOwner = Parser.ParseCells("Meshes/testmesh_45e_owner.txt");
	//auto DataNeighbour= Parser.ParseCells("Meshes/testmesh_45e_neighbour.txt");
	//int cellCount = Parser.ParseCellCount("Meshes/testmesh_45e_owner.txt");
	//auto DataBoundary = Parser.ParseBoundary("Meshes/testmesh_45e_boundary.txt");

	//auto DataIndex = Parser.ParseFaces("Meshes/sphere_3k_faces.txt");
	//auto DataVertex = Parser.ParsePoints("Meshes/sphere_3k_points.txt");
	//auto DataOwner = Parser.ParseCells("Meshes/sphere_3k_owner.txt");
	//auto DataNeighbour = Parser.ParseCells("Meshes/sphere_3k_neighbour.txt");
	//int cellCount = Parser.ParseCellCount("Meshes/sphere_3k_owner.txt");
	//auto DataBoundary = Parser.ParseBoundary("Meshes/sphere_3k_boundary.txt");

	auto DataIndex = Parser.ParseFaces("Meshes/spheroid_45k_faces.txt");
	auto DataVertex = Parser.ParsePoints("Meshes/spheroid_45k_points.txt");
	auto DataOwner = Parser.ParseCells("Meshes/spheroid_45k_owner.txt");
	auto DataNeighbour = Parser.ParseCells("Meshes/spheroid_45k_neighbour.txt");
	int cellCount = Parser.ParseCellCount("Meshes/spheroid_45k_owner.txt");
	auto DataBoundary = Parser.ParseBoundary("Meshes/spheroid_45k_boundary.txt");

	//auto DataIndex = Parser.ParseFaces("Meshes/flange_mf_282k_faces.txt");
	//auto DataVertex = Parser.ParsePoints("Meshes/flange_mf_282k_points.txt");
	//auto DataOwner = Parser.ParseCells("Meshes/flange_mf_282k_owner.txt");
	//auto DataNeighbour = Parser.ParseCells("Meshes/flange_mf_282k_neighbour.txt");
	//int cellCount = Parser.ParseCellCount("Meshes/flange_mf_282k_owner.txt");
	//auto DataBoundary = Parser.ParseBoundary("Meshes/flange_mf_282k_boundary.txt");

	//VISUALIZATIONS-----------------------------------------------------------------------------------------------------------
	//VisIndxElement<shared_ptr<DataVector<float>>, shared_ptr<vector<DataVector<int>>>> vizElement(DataVertex, DataIndex);
	VisIndxElement<	shared_ptr<DataVector<float>>,
					shared_ptr<vector<DataVector<int>>>,
					shared_ptr<DataVector<int>>,
					shared_ptr<DataVector<int>>,
					shared_ptr<DataVector<int>>> vizElement(DataVertex, DataIndex, DataOwner, DataNeighbour, DataBoundary, cellCount);

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

