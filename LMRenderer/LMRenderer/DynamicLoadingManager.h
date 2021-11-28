#pragma once
#include "VisGroup.h"
#include "Globals.h"
#include "VisMeshZone.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class DynamicLoadingManager
{
private:
	//shared_ptr<VisGroup> visZones;
	vector<shared_ptr<VisMeshZone>> visZones;
	vector<shared_ptr<VisMeshZone>> visibleZones;
	//vector<VisMeshZone*>  visibleZones;
	vector<shared_ptr<VisMeshZone>> unloadableZones;
	//vector<VisMeshZone*> unloadableZones;
	
	int trisPerZone;

	// Estos valores son en cantidad de triangulos, deberian llamarse TrisMemoryLimit, TrisMemoryNeeded etc. ?
	int memoryLimit = DYNAMIC_LOADING_LIMIT;
	int memoryNeeded = 0;
	int memoryOcupied = 0;

public:
	DynamicLoadingManager(shared_ptr<VisGroup> zones);
	DynamicLoadingManager(vector<shared_ptr<VisMeshZone>> onlyzones);
	//DynamicLoadingManager(VisGroup* meshzones);
	//void SetVisualizationZones(VisGroup* root);
	void Update();
	void CalcOcupiedMemory();
	void LoadZones();
	void UnloadZones();
	int GetMemoryNeeded() { return memoryNeeded; }
	int GetMemoryOccupied() { return memoryOcupied; }
};

