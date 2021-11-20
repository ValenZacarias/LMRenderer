#pragma once
#include "VisGroup.h"
#include "VisMeshZone.h"

class DynamicLoadingManager
{
private:
	VisGroup* visZones;
	std::vector<VisMeshZone> visibleZones;
	std::vector<VisMeshZone> unloadableZones;
	
	int trisPerZone;

	// Estos valores son en cantidad de triangulos, deberian llamarse TrisMemoryLimit, TrisMemoryNeeded etc. ?
	int memoryLimit;
	int memoryNeeded;
	int memoryOcupied;

public:
	DynamicLoadingManager();
	//DynamicLoadingManager(VisGroup* meshzones);
	void SetVisualizationZones(VisGroup* root);
	void Update();
	void CalcTrisPerZone();
	void CalcOcupiedMemory();
	void LoadZones();
	void UnloadZones();

};

