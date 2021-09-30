#pragma once
#include <glm/glm.hpp> 
#include "Face.h"

class PAFaceIndexTriangulate
{
public:
	PAFaceIndexTriangulate() {}

	DataVector<glm::vec3> Process(DataVector<glm::vec3>& vertex, DataVector<Face>& faces)
	{
		//Este metodo toma 4 indices de un quad y saca 6 indices para los 2 triangulos que lo componen
		//Debariamos hacer un reserve aca, asi no resizeamos multiples veces
		DataVector<glm::vec3> trisData(POINT);

		int p0Index;
		for (int i = 0; i < faces.GetSize(); i ++) //Agarro el quad completo
		{
			p0Index = faces.GetData(i).GetP0();
			for (int j = 0; j < faces.GetData(i).GetCount() ; j++)
			{
				trisData.SetData(vertex.GetData( p0Index + j ));
			}
		}

		return trisData;
	}


private:

};

