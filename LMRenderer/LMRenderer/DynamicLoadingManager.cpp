#include "DynamicLoadingManager.h"

using namespace std;

DynamicLoadingManager::DynamicLoadingManager(shared_ptr<VisGroup> zones)
{
	//visZones = zones;
	//visibleZones.reserve(visZones->visualizations.size());
	//unloadableZones.reserve(visZones->visualizations.size());
}

// Este constructor toma un vector de Zonas, no usa precisamente el arbol de visualizaciones completo ya que
// esto trajo problemas al tene que castear las visualizaciones a visMeshZones
DynamicLoadingManager::DynamicLoadingManager(vector<shared_ptr<VisMeshZone>> onlyzones)
{
	visZones = onlyzones;
	visibleZones.reserve(visZones.size());
	unloadableZones.reserve(visZones.size());
}

void DynamicLoadingManager::Update()
{
	if (!DYNAMIC_LOADING_ENABLED) return;
	memoryLimit = DYNAMIC_LOADING_LIMIT;

	//1.1 llenar lista visibleZones
	//1.2 llenar lista unloadableZones (con las no visibles)
	for (int i = 0; i < visZones.size() ; i++)
	{
		if (visZones[i]->IsVisible())
		{
			visibleZones.push_back(visZones[i]);
		}
		else
		{
			unloadableZones.push_back(visZones[i]);
		}
	}

	//2.1 Calcular memoria por zona
	if (visibleZones.size() > 0){ trisPerZone = memoryLimit / visibleZones.size(); }

	//2.2 Calcular memoria ocupada - PRE LOADING
	CalcOcupiedMemory();

	//3.1 Load
	//3.2 llenar lista unloadableZones (con las visibles cargadas con > trisPerZone)
	LoadZones();

	//4. mientras occupiedMemory + neededMemory > memLimit->Descargar
	int it = 0;
	while (memoryNeeded + memoryOcupied > memoryLimit)
	{
		memoryOcupied -= unloadableZones[it]->UnloadLastLevel();
		
		if (it == unloadableZones.size() - 1)
			it = 0;
		else	
			it++;
	}

	// 5. Clear data
	visibleZones.clear();
	unloadableZones.clear();
	memoryNeeded = 0;
	memoryOcupied = 0;
}


void DynamicLoadingManager::CalcOcupiedMemory()
{
	memoryOcupied = 0;

	for (int i = 0; i < visZones.size(); i++)
	{
		memoryOcupied += visZones[i]->GetTrisLoaded();
	}
}

void DynamicLoadingManager::LoadZones()
{
	for (int i = 0; i < visibleZones.size(); i++)
	{
		__nop();
		if (visibleZones[i]->GetTrisLoaded() > trisPerZone)
		{
			// zonas excedidadas
			unloadableZones.push_back(visibleZones[i]);
		}
		else 
		{
			int level = visibleZones[i]->GetPossibleLevel(trisPerZone);
			if (level != -1)
			{
				if(level > visibleZones[i]->GetCurrentLevel())
				{
					int memBeforeLoad = visibleZones[i]->GetTrisLoaded();
					//cout << "LOADING ZONE " << i << " LEVEL " << level << endl;
					visibleZones[i]->LoadLevel(level);
					memoryNeeded += (visibleZones[i]->GetTrisLoaded() - memBeforeLoad);
				}
			}
		}
	}

}
