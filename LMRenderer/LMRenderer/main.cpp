#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "shader_s.h"
#include "Globals.h"
#include "PMeshParser.h"
#include "DrawLine.h"
#include "DrawPoint.h"

#include "Tool.h"
#include "ToolFPSCamera.h"
#include "ToolOrbitCamera.h"

#include "GLFWCanvas.h"
#include "ZoneGenerator.h"

#include "VisBase.h"
#include "VisGroup.h"
#include "VisGrid.h"
#include "VisPrimitive.h"
#include "VisCellFacesTesting.h"
#include "VisCell_bin.h"
#include "VisMeshZone.h"

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
#include "PACalcBoundingBox.h"

#include "main.h"

float lastFrame = 0.0f;

GLFWwindow* window = NULL;

int main()
{
	ZoneGenerator zoneGenerator;

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
	PACalcBoundingBox calcBoundingBox;

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

	// Data Processing PIPELINE

	// Initialize faceData with null faces
	/*auto faceData = make_shared<DataVector<Face>>(FACE);
	createFaces.Process(*DataOwner, *faceData);

	// Generate cells
	auto cellData = make_shared<DataVector<Cell>>(CELL);
	generateCells.Process(*DataOwner, *DataNeighbour, *DataBoundary, *faceData, *cellData, cellCount);

	// Triangulate with Face data CREATION and Area Calc
	auto triVertexData = make_shared<DataVector<glm::vec3>>(POINT);
	DataVector<float> faceAreaData(FLOATVAL);
	triangulate.Process(*DataVertex, *DataIndex, *triVertexData, *faceData, faceAreaData);

	// Calc Bounding Box
	vector<glm::vec3> BBVertices{};
	calcBoundingBox.Process(*DataVertex, BBVertices);

	// First sample
	auto faceDataBuffer = make_shared<DataVector<Face>>(FACE);
	auto cellDataSample = make_shared<DataVector<Cell>>(CELL);

	uniformSample.Process(cellData, cellDataSample, 0.1);

	reconstructCells.Process(*faceData, *faceDataBuffer, *cellDataSample);

	auto triVertexBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, "bin/trivertex_test.bin");

	if (!triVertexBuffer->FileExists())
	{
		faceIndexTriangulate.Process(*triVertexBuffer, *triVertexData, *faceDataBuffer);
		triVertexBuffer->EndWrite();
		triVertexBuffer->SaveFile();
	}
	VisCell_bin<shared_ptr<DataBinFile<glm::vec3>>> Vis_Sample_1(triVertexBuffer, cellCount, BBVertices);*/

	//// Second sample
	//auto faceDataBuffer2 = make_shared<DataVector<Face>>(FACE);
	//auto cellDataSample2 = make_shared<DataVector<Cell>>(CELL);
	//uniformSample.Process(cellData, cellDataSample2, 0.1);

	//reconstructCells.Process(*faceData, *faceDataBuffer2, *cellDataSample2);
	////reconstructCells.Process(*faceData, *faceDataBuffer2, *cellData);

	//auto triVertexBuffer2 = make_shared<DataBinFile<glm::vec3>>(POINT, "bin/trivertex_2_test.bin");

	//if (!triVertexBuffer2->FileExists())
	//{
	//	faceIndexTriangulate.Process(*triVertexBuffer2, *triVertexData, *faceDataBuffer2);
	//	triVertexBuffer2->EndWrite();
	//	triVertexBuffer2->SaveFile();
	//}

	//VisCell_bin<shared_ptr<DataBinFile<glm::vec3>>> Vis_Sample_2(triVertexBuffer2, cellCount);

	//float gridScale = glm::length(BBVertices[1] - BBVertices[0]);

	//auto VisZone_1 = zoneGenerator.GenerateZone("Meshes/spheroid_45k");
	//vector<shared_ptr<VisMeshZone>> zones = zoneGenerator.GenerateRepeatedZones("Meshes/spheroid_45k", 3, 3, 3);
	vector<shared_ptr<VisMeshZone>> zones = zoneGenerator.GenerateRepeatedZones("Meshes/sphere_3k", 16, 4, 4);

	float gridScale = 5.0f;
	
	//VisGrid VisGrid_1 = VisGrid(gridScale);
	auto VisGrid_1 = make_shared<VisGrid>(gridScale);

	VisGroup MainGroupViz = VisGroup();
	
	for (int i = 0; i < zones.size(); i++)
	{
		MainGroupViz.shared_visualizations.push_back(zones[i]);
	}

	MainGroupViz.shared_visualizations.push_back(VisGrid_1);

	canvas.SetupContext(&MainGroupViz);

	float currentFrame = 0.0f;
	float timer = 0.0f;
	std::stringstream ss;
	int nbFrames = 0;
	double fps;
	char title_string[100] = "LMRenderer";

	bool LoadOnce = true;
	bool SendToGPUOnce = true;
	bool UnloadFromGPUOnce = true;
	bool UnloadFromRAMOnce = true;

	float originLinesLenght = 5.0f;

	// ORIGIN LINES
	DrawLine xAxis(glm::vec3(0.0f, 0.0f, 0.0f), originLinesLenght * glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, glm::vec3(0.9, 0.1, 0.1));
	DrawLine yAxis(glm::vec3(0.0f, 0.0f, 0.0f), originLinesLenght * glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, glm::vec3(0.1, 0.9, 0.1));
	DrawLine zAxis(glm::vec3(0.0f, 0.0f, 0.0f), originLinesLenght * glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, glm::vec3(0.1, 0.1, 0.9));

	int triangleLimit = 50000;

	//RENDER LOOP! -----------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		timer += deltaTime;

		/*if ((int)timer > 0)
		{
			// Vis State change test
			if ((int)timer%7 == 0 && LoadOnce)
			{
				cout << "try loading 1" << endl;
				int levelIndex = zones[0]->GetPossibleLevel(triangleLimit);
				if ( levelIndex != -1) { zones[0]->LoadLevel(levelIndex); }
				else { cout << "Cannot load lv" << levelIndex << endl; }

				LoadOnce = false;
			}

			if ((int)timer%15 == 0 && SendToGPUOnce)
			{
				cout << "try loading 2" << endl;
				int levelIndex = zones[1]->GetPossibleLevel(triangleLimit);
				if (levelIndex != -1) { zones[1]->LoadLevel(levelIndex); }
				else { cout << "Cannot load lv" << levelIndex << endl; }

				SendToGPUOnce = false;
			}
		}*/
		
		canvas.KeyboardHandler(window);
		canvas.Render();
		

		// DRAW ORIGIN LINES
		xAxis.Render(&canvas.currentCamera);
		yAxis.Render(&canvas.currentCamera);
		zAxis.Render(&canvas.currentCamera);

		glfwSwapBuffers(window);
		glfwPollEvents();

		// FPS Counter
		if (deltaTime > 0.015)
		{
			fps = round((double)nbFrames / deltaTime);
			sprintf_s(title_string, "FPS: %.1f - time: %.2i", fps, (int)timer);
			glfwSetWindowTitle(window, title_string);
			lastFrame = currentFrame;
			nbFrames = 0;
		}

		nbFrames++;
	}

	//canvas.~GLFWCanvas();

	glfwTerminate();
	return 0;
}

void RenderUI()
{

}
