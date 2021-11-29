#include "DynamicLoadingManager.h"

using namespace std;

DynamicLoadingManager::DynamicLoadingManager(shared_ptr<VisGroup> zones)
{
	//visZones = zones;
	//visibleZones.reserve(visZones->visualizations.size());
	//unloadableZones.reserve(visZones->visualizations.size());
}

DynamicLoadingManager::DynamicLoadingManager(vector<shared_ptr<VisMeshZone>> onlyzones)
{
	visZones = onlyzones;
	visibleZones.reserve(visZones.size());
	unloadableZones.reserve(visZones.size());
}

// Esto no podriamos hacerlo 1 vez cada x tiempo/frames? o deberia correr frame a frame?
void DynamicLoadingManager::Update()
{
	if (!DYNAMIC_LOADING_ENABLED) return;
	
	//1.1 llenar lista visibleZones
	//1.2 llenar lista unloadableZones (con las no visibles)
	//for (int i = 0; i < visZones->shared_visualizations.size() ; i++)
	for (int i = 0; i < visZones.size() ; i++)
	{
		//shared_ptr<VisMeshZone> actualZone;

		// visualizations[i] is also a sharedptr

		//if (visZones->shared_visualizations[i]->IsVisible())
		if (visZones[i]->IsVisible())
		{
			//visibleZones.push_back(static_pointer_cast<VisMeshZone>(visZones->shared_visualizations[i]));
			//actualZone = static_pointer_cast<VisMeshZone>(visZones->shared_visualizations[i]);

			visibleZones.push_back(visZones[i]);
		}
		else
		{
			//unloadableZones.push_back(static_pointer_cast<VisMeshZone>(visZones->shared_visualizations[i]));
			//actualZone = static_pointer_cast<VisMeshZone>(visZones->shared_visualizations[i]);
			//El problema es que esta casteando la visualizacion de grilla como una vis mesh zone llena de basura

			unloadableZones.push_back(visZones[i]);
		}

		//__nop();
	}

	if (!visZones[1]->IsVisible())
		//__nop();

	if (visibleZones.size() > 0)
		trisPerZone = memoryLimit / visibleZones.size();

	//4. Calcular memoria ocupada - PRE LOADING
	CalcOcupiedMemory();

	//3.1 Load
	//3.2 llenar lista unloadableZones (con las visibles cargadas con > trisPerZone)
	LoadZones();

	//cout << "OCCUP: " << memoryOcupied << " || NEEDED: " << memoryNeeded << endl;

	//5. mientras occupiedMemory + neededMemory > memLimit->Descargar
	int it = 0;
	while (memoryNeeded + memoryOcupied > memoryLimit)
	{
		// estamos teniendo un problema donde pareciera que el nivel cero se carga 2 veces entonces hay visualizaciones que dicen que
		// no tienen mas nada para descargar dado que lv = -1 pero aun tienen datos cargados osea triCount = 1200
		memoryOcupied -= unloadableZones[it]->UnloadLastLevel();
		
		if (it == unloadableZones.size() - 1)
			it = 0;
		else	
			it++;
	}

	// 6. Clear data
	visibleZones.clear();
	unloadableZones.clear();
	memoryNeeded = 0;
	memoryOcupied = 0;
}


void DynamicLoadingManager::CalcOcupiedMemory()
{
	/*Calcular cuanta memoria hay ocupada -- SUMA DE TRIS LOADED
		MO = sum(LV[i].cant_cargados) ) + sum(LNV[i].cant_cargados) )*/
	memoryOcupied = 0;
	//for (int i = 0; i < visZones->shared_visualizations.size(); i++)
	//for (int i = 0; i < unloadableZones.size(); i++)
	for (int i = 0; i < visZones.size(); i++)
	{
		//memoryOcupied += static_pointer_cast<VisMeshZone>(visZones->shared_visualizations[i])->GetTrisLoaded();
		memoryOcupied += visZones[i]->GetTrisLoaded();
		//memoryOcupied += unloadableZones[i]->GetTrisLoaded();
	}
}

void DynamicLoadingManager::LoadZones()
{
	for (int i = 0; i < visibleZones.size(); i++)
	{
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
					cout << "LOADING ZONE " << i << " LEVEL " << level << endl;
					visibleZones[i]->LoadLevel(level);
					//memoryNeeded += visibleZones[i]->GetTrisLoaded();
					memoryNeeded += (visibleZones[i]->GetTrisLoaded() - memBeforeLoad);
				}
			}
		}
	}

}

void DynamicLoadingManager::UnloadZones()
{
	/*
	mientras MO + MM > MEM // Mem Max elementos en memoria
		pedirle a LNV[i] que descargue un nivel 
			//La lista LNV, son Cercanas y Lejanas, LNV se puede preordenar poniendo las zonas lejanas primero,
			// Tambien se pueden ordenar por las que mas ocupan etc. Todos estos ordenamientos son preprocesos al while
		si ya quedo en el minimo, sacarlo de LVN
		si no hay LNV, pedirle a los LV que tengan mas de lo necesario 
			// ARMAR UNA LISTA DE VISIBLES CON EXCESO, SI NO HAY NO VISIBLES, PEDIRLE A LAS VISIBLES CON EXCESO

		// PARA ESTE CASO SE PODRIA PLANTEAR UNA UNICA LISTA DE: "DESCARGABLES" QUE INCLUYA LAS OCULTAS Y LAS EXCEDIDAS
		// ESTA LISTA SE PODRIA ORDENAR POR ALGUN CRITERIO (DISTANCIA A LA CAMARA, CANTIDAD DE DATOS CARGADOS, NO-VISIBLE/VISIBLE(EXCEDIDA))
		// PARA ELEGIR QUIEN ES LA PRIMERA EN DESCARGAR*/

}