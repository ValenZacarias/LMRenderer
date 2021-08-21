#include "PACalcNormals.h"
#include "DataVectorTest.h"
#include <glm/glm.hpp> 

PACalcNormals::PACalcNormals()
{
}

DataVectorTest<float> PACalcNormals::Process(std::shared_ptr<DataVectorTest<float>> vertex, std::shared_ptr<DataVectorTest<int>> elementIndx, int faceVertexNum)
{
	DataVectorTest<float> normalData = DataVectorTest<float>(FLOATVAL);
	
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
	
	glm::vec3 v1;
	glm::vec3 v2;
	
	glm::vec3 normal;

	int index0;
	int index1;
	int index2;

	for (int i = 0; i < elementIndx->GetSize(); i += faceVertexNum)
	{
		index0 = faceVertexNum;			//First face index
		index1 = faceVertexNum + 1;		//Second face index
		index2 = faceVertexNum * 2 - 1;	//Last face index

		p1 = glm::vec3(vertex->GetData(index0), 
						vertex->GetData(index0 + 1),
						vertex->GetData(index0 + 2));

		p2 = glm::vec3(vertex->GetData(index1),
						vertex->GetData(index1 + 1),
						vertex->GetData(index1 + 2));

		p2 = glm::vec3(vertex->GetData(index2),
						vertex->GetData(index2 + 1),
						vertex->GetData(index2 + 2));

		v1 = p2 - p1;
		v2 = p3 - p1;

		normal = glm::cross(v1, v2);

		normalData.SetData(normal[0]);
		normalData.SetData(normal[1]);
		normalData.SetData(normal[2]);

	}

	return normalData;
}

DataVectorTest<float> PACalcNormals::Process(std::shared_ptr<DataVectorTest<float>> vertex)
{
	DataVectorTest<float> normalData = DataVectorTest<float>(FLOATVAL);

	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 normal;

	for (int i = 0; i < vertex->GetSize(); i += 9)
	{
		p1 = glm::vec3(vertex->GetData(i), vertex->GetData(i+1), vertex->GetData(i+2));
		p2 = glm::vec3(vertex->GetData(i+3), vertex->GetData(i+4), vertex->GetData(i+5));
		p3 = glm::vec3(vertex->GetData(i+6), vertex->GetData(i+7), vertex->GetData(i+8));

		v1 = p2 - p1;
		v2 = p3 - p1;

		normal = glm::cross(v1, v2);

		//Agrego 3 veces la normal dado que es la misma para los 3 vertices del triangulo
		normalData.SetData(normal[0]);
		normalData.SetData(normal[1]);
		normalData.SetData(normal[2]);

		normalData.SetData(normal[0]);
		normalData.SetData(normal[1]);
		normalData.SetData(normal[2]);

		normalData.SetData(normal[0]);
		normalData.SetData(normal[1]);
		normalData.SetData(normal[2]);
	}

	return normalData;
}


