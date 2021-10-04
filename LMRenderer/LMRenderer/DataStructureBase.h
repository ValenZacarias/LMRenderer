#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

enum DATATYPE { POINT, VECTOR, NORMAL, INDEX, CENTROID, INTVAL, FLOATVAL, POLYINDX, FACE, CELL, BOUNDS};

//El contador general de ids se puede poner como static aca, para no tirarselo a globals
//Puede quedar la Base para el tema de IDS

class DataStructureBase
{
private:
	int ID;
	DATATYPE type;

public:
	DataStructureBase(DATATYPE t) 
	{
		type = t;
		ID = DataStructureCurrentIndex;
		DataStructureCurrentIndex++;
	};

	int GetID() { return ID; };
	int GetDataType() { return type; };
};