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
#include "PMeshParser.h"

#include "Tool.h"
#include "ToolFPSCamera.h"
#include "ToolOrbitCamera.h"

#include "GLFWCanvas.h"

#include "VisBase.h"
#include "VisGroup.h"
#include "VisGrid.h"
#include "VisPrimitive.h"
#include "VisCellFacesTesting.h"
#include "VisCell_bin.h"

#include "DataStructureBase.h"
#include "DataVector.h"
#include "DataBinFile.h"
#include "Face.h"
#include "Cell.h"

#include "PATriangulate.h"
#include "PACalcNormals.h"
#include "PAUniformSample.h"
#include "PAInvTransformSample.h"
#include "PAFaceIndexTriangulate.h"
#include "PACalcCDF.h"
#include "PAGenerateCells.h"
#include "PAReconstructCells.h"
#include "PACreateFaceData.h"
#include "PACalcCellCentroid.h"
#include "PACalcCellVolume.h"

#include "main.h"

float lastFrame = 0.0f;

GLFWwindow* window = NULL;

int main()
{
	PATriangulate triangulate;
	PACalcNormals calcnormals;
	PAUniformSample uniformSample;
	PAInvTransformSample invTransformSample;
	PAFaceIndexTriangulate faceIndexTriangulate;
	PACalcCDF calcCDF;
	PAGenerateCells generateCells;
	PAReconstructCells reconstructCells;
	PACreateFaceData createFaces;
	PACalcCellCentroid calcCellCentroid;
	PACalcCellVolume calcCellVolume;

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
	//auto DataOwner = Parser.ParseCells("Meshes/testmesh_45e_owner.txt");
	//auto DataNeighbour= Parser.ParseCells("Meshes/testmesh_45e_neighbour.txt");
	//int cellCount = Parser.ParseCellCount("Meshes/testmesh_45e_owner.txt");
	//auto DataBoundary = Parser.ParseBoundary("Meshes/testmesh_45e_boundary.txt");

	auto DataIndex = Parser.ParseFaces("Meshes/sphere_3k_faces.txt");
	auto DataVertex = Parser.ParsePoints("Meshes/sphere_3k_points.txt");
	auto DataOwner = Parser.ParseCells("Meshes/sphere_3k_owner.txt");
	auto DataNeighbour = Parser.ParseCells("Meshes/sphere_3k_neighbour.txt");
	int cellCount = Parser.ParseCellCount("Meshes/sphere_3k_owner.txt");
	auto DataBoundary = Parser.ParseBoundary("Meshes/sphere_3k_boundary.txt");

	//auto DataIndex = Parser.ParseFaces("Meshes/spheroid_45k_faces.txt");
	//auto DataVertex = Parser.ParsePoints("Meshes/spheroid_45k_points.txt");
	//auto DataOwner = Parser.ParseCells("Meshes/spheroid_45k_owner.txt");1
	//auto DataNeighbour = Parser.ParseCells("Meshes/spheroid_45k_neighbour.txt");
	//int cellCount = Parser.ParseCellCount("Meshes/spheroid_45k_owner.txt");
	//auto DataBoundary = Parser.ParseBoundary("Meshes/spheroid_45k_boundary.txt");

	//auto DataIndex = Parser.ParseFaces("Meshes/flange_mf_282k_faces.txt");
	//auto DataVertex = Parser.ParsePoints("Meshes/flange_mf_282k_points.txt");
	//auto DataOwner = Parser.ParseCells("Meshes/flange_mf_282k_owner.txt");
	//auto DataNeighbour = Parser.ParseCells("Meshes/flange_mf_282k_neighbour.txt");
	//int cellCount = Parser.ParseCellCount("Meshes/flange_mf_282k_owner.txt");
	//auto DataBoundary = Parser.ParseBoundary("Meshes/flange_mf_282k_boundary.txt");

	// Data Processing PIPELINE

	// Initialize faceData with null faces
	auto faceData = make_shared<DataVector<Face>>(FACE);
	createFaces.Process(*DataOwner, *faceData);

	// Generate cells
	auto cellData = make_shared<DataVector<Cell>>(CELL);
	generateCells.Process(*DataOwner, *DataNeighbour, *DataBoundary, *faceData, *cellData, cellCount);

	//Triangulate with Face data CREATION and Area Calc
	auto triVertexData = make_shared<DataVector<glm::vec3>>(POINT);
	DataVector<float> faceAreaData(FLOATVAL);
	triangulate.Process(*DataVertex, *DataIndex, *triVertexData, *faceData, faceAreaData);

	auto faceDataBuffer = make_shared<DataVector<Face>>(FACE);

	reconstructCells.Process(*faceData, *faceDataBuffer, *cellData);

	auto triVertexBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, "bin/trivertex_test.bin");

	if (!triVertexBuffer->FileExists())
	{
		faceIndexTriangulate.Process(*triVertexBuffer, *triVertexData, *faceDataBuffer);
		triVertexBuffer->EndWrite();
		triVertexBuffer->SaveFile();
	}

	//VISUALIZATIONS-----------------------------------------------------------------------------------------------------------
	//VisCellFacesTesting<	shared_ptr<DataVector<float>>,
	//				shared_ptr<vector<DataVector<int>>>,
	//				shared_ptr<DataVector<int>>,
	//				shared_ptr<DataVector<int>>,
	//				shared_ptr<DataVector<int>>> VisCellFacesTesting(DataVertex, DataIndex, DataOwner, DataNeighbour, DataBoundary, cellCount);

	
	VisCell_bin<shared_ptr<DataBinFile<glm::vec3>>>VisCell_bin(triVertexBuffer, cellCount);

	VisGrid vizGrid = VisGrid();
	VisGroup MainGroupViz = VisGroup();
	//MainGroupViz.visualizations.push_back(&VisCellFacesTesting);
	MainGroupViz.visualizations.push_back(&VisCell_bin);
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

