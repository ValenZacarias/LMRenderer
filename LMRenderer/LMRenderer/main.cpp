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

#include "DynamicLoadingManager.h"

#include "main.h"

float lastFrame = 0.0f;

GLFWwindow* window = NULL;

int main()
{
	DYNAMIC_LOADING_LIMIT = 30000;

	ZoneGenerator zoneGenerator;

	// CANVAS SETUP ----------------------------------------------------------------------------------------------------------
	GLFWCanvas canvas = GLFWCanvas(screenWidth, screenHeight);
	window = canvas.Init();

	ToolFPSCamera cameraTool = ToolFPSCamera(&canvas.currentCamera);
	//ToolOrbitCamera cameraTool = ToolOrbitCamera(&canvas.currentCamera);
	canvas.SetCurrentTool(&cameraTool);

	// ZONE GENERATION -------------------------------------------------------------------------------------------------------
	//auto VisZone_1 = zoneGenerator.GenerateZone("Meshes/spheroid_45k");
	//vector<shared_ptr<VisMeshZone>> zones = zoneGenerator.GenerateRepeatedZones("Meshes/spheroid_45k", 3, 3, 3);
	vector<shared_ptr<VisMeshZone>> zones = zoneGenerator.GenerateRepeatedZones("Meshes/sphere_3k", 16, 4, 4);

	// VIS INIT -------------------------------------------------------------------------------------------------------
	float gridScale = 5.0f;
	auto VisGrid_1 = make_shared<VisGrid>(gridScale);
	//VisGrid VisGrid_1 = VisGrid(gridScale);

	auto MainGroupViz = make_shared<VisGroup>();
	for (int i = 0; i < zones.size(); i++) { MainGroupViz->shared_visualizations.push_back(zones[i]); }
	MainGroupViz->shared_visualizations.push_back(VisGrid_1);
	canvas.SetupContext(MainGroupViz);

	// MANAGER INIT -----------------------------------------------------------------------------------------------------------

	DynamicLoadingManager DLManager(zones);

	// MISC DATA
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

	//int triangleLimit = 50000;

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

		DLManager.Update();

		canvas.Render();

		//RenderDLManagerUI(DLManager.GetMemoryOccupied(), DLManager.GetMemoryNeeded());

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

	glfwTerminate();
	return 0;
}
