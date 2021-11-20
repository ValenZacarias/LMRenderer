#include "DynamicLoadingManager.h"

DynamicLoadingManager::DynamicLoadingManager()
{
	//visZones = meshzones;
}

void SetVisualizationZones(VisGroup* root) 
{
	
}

void DynamicLoadingManager::Update()
{
	// Esto no podriamos hacerlo 1 vez cada x tiempo/frames? o deberia correr frame a frame?
	/*
	1.1 llenar lista visibleZones
	1.2 llenar lista unloadableZones (con las no visibles)
	
	2. Calcular TrisPerZone

	3.1 Load
	3.2 llenar lista unloadableZones (con las visibles cargadas con > trisPerZone)

	4. Calcular memoria ocupada

	5. mientras occupiedMemory + neededMemory > memLimit -> Descargar

	*/
}

void DynamicLoadingManager::CalcTrisPerZone()
{
	if (visibleZones.size() > 0)
		trisPerZone = memoryLimit / visibleZones.size();
}

void DynamicLoadingManager::CalcOcupiedMemory()
{
	/*Calcular cuanta memoria hay ocupada -- SUMA DE TRIS LOADED
		MO = sum(LV[i].cant_cargados) ) + sum(LNV[i].cant_cargados) )*/
}

void DynamicLoadingManager::LoadZones()
{
	/*for each visible
	{
		int level = zone_0.GetPossibleLevel(TrisPerZone);
		int memoriaNecesariaPorZona = zone_0.loadLevel(level);

		neededMemory += memoriaNecesariaPorZona;
	}*/
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