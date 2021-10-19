#pragma once
#include <glm/glm.hpp> 
#include "Face.h"

class PATriangulate
{
public:
	PATriangulate() {}

	//vector<int> triangulationIndices = {0, 1, 2, 2, 3, 0, 3, 4, 0, 4, 5, 0, 6}

	// Overload that calculate face areas and face index
	template <typename TVertex, typename TIndex>
	void Process(TVertex& vertex, 
				 TIndex& elemIndxVector, 
				 DataVector<glm::vec3>& trisVertex, 
				 DataVector<Face>& faceIndex, 
				 DataVector<float>& faceArea)
	{
		int vertexCount = 0;
		int trisPerPoly = 1;
		for (int i = 3; i < elemIndxVector.size(); i++)
		{
			// elementIndexVector[i].GetSize()/i = cantidad de elementos, * trisPerPoly = tris totales p/todos los elem
			// * 3 = 3 vertices por tri * 3 (floats por vert) -> cant floats totales para todo el DataStructure
			vertexCount += (elemIndxVector[i].GetSize() / i) * trisPerPoly * 3;// * 3;
			trisPerPoly++;
		}

		trisVertex.ReserveData(vertexCount);
		//Face faceData;

		for (int i = 3; i < elemIndxVector.size(); i++)
		{
			auto& indxData = elemIndxVector[i];
			float triArea = 0;

			int triCount = i - 2;

			for (int j = 0; j < indxData.GetSize(); j += (i + 1))
			{
				// Generating triangles for one face of i vertices and i - 2 triangles
				trisVertex.GetSize() != 0 ?
					faceIndex.GetData(indxData.GetData(j)).SetP0(trisVertex.GetSize()) : faceIndex.GetData(indxData.GetData(j)).SetP0(0);

				faceIndex.GetData(indxData.GetData(j)).SetCount( 3 * triCount);

				for (int k = 0; k < triCount; k++)
				{
					if (k == 0)
					{
						AddTri(vertex, indxData, trisVertex, j + 1, 0, 1, 2);
						triArea += CalcTriArea(vertex, indxData, j + 1, 0, 1, 2);
					}
					else
					{
						AddTri(vertex, indxData, trisVertex,	 j + 1, k + 1, k + 2, 0); // ej: triangulo 2 de un quad = 2, 3, 0
						triArea += CalcTriArea(vertex, indxData, j + 1, k + 1, k + 2, 0);
					}
				}

				faceArea.SetData(triArea);
				triArea = 0;
			}
		}
	}

private:
	glm::vec3 GetPoint(DataVector<float>& vertexData, int index)
	{
		return glm::vec3(vertexData.GetData(3 * index), vertexData.GetData(3 * index + 1), vertexData.GetData(3 * index + 2));
	}
	
	template <typename TVertex, typename TIndex>
	void AddTri(TVertex& vertexData, TIndex& indexData, DataVector<glm::vec3>& trisVertex, int firstIndex, int i0, int i1, int i2)
	{
		trisVertex.SetData(GetPoint(vertexData, indexData.GetData(firstIndex + i0)));
		trisVertex.SetData(GetPoint(vertexData, indexData.GetData(firstIndex + i1)));
		trisVertex.SetData(GetPoint(vertexData, indexData.GetData(firstIndex + i2)));
	}

	template <typename TVertex, typename TIndex>
	float CalcTriArea(TVertex& vertexData, TIndex& indexData, int firstIndex, int i0, int i1, int i2)
	{
		float triArea = 0.5f * glm::length(glm::cross(
		GetPoint(vertexData, indexData.GetData(firstIndex + i1)) - GetPoint(vertexData, indexData.GetData(firstIndex + i0)),
		GetPoint(vertexData, indexData.GetData(firstIndex + i2)) - GetPoint(vertexData, indexData.GetData(firstIndex + i1))));
		
		return triArea;
	}
};

