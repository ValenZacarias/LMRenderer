#pragma once
#include <glm/glm.hpp> 
#include "Face.h"

class PATriangulate
{
public:
	PATriangulate() {}

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
		Face faceData;

		for (int i = 3; i < elemIndxVector.size(); i++)
		{
			auto& indxData = elemIndxVector[i];
			float triArea = 0;

			//Hacer un _expect(vpos.getType()==PointsType) para Esperar valores mayores a 2;
			switch (i)
			{
			case 3:
				for (int j = 0; j < indxData.GetSize(); j += 3)
				{
					
					trisVertex.GetSize() != 0 ? faceData.SetP0(trisVertex.GetSize()) : faceData.SetP0(0);
					faceData.SetCount(3);
					faceIndex.SetData(faceData);

					AddTri(vertex, indxData, trisVertex, j, 0, 1, 2);
					triArea = +CalcTriArea(vertex, indxData, j, 0, 1, 2);

					faceArea.SetData(triArea);
					triArea = 0;
				}
				break;
			case 4:
				for (int j = 0; j < indxData.GetSize(); j += 4)
				{
					trisVertex.GetSize() != 0 ? faceIndex.GetData((int)j / 4).SetP0(trisVertex.GetSize()) : faceIndex.GetData((int)j / 4).SetP0(0);

					faceIndex.GetData((int)j / 4).SetCount(6);
					
					AddTri(vertex, indxData, trisVertex, j, 0, 1, 2);
					triArea = +CalcTriArea(vertex, indxData, j, 0, 1, 2);

					AddTri(vertex, indxData, trisVertex, j, 2, 3, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 2, 3, 0);

					faceArea.SetData(triArea);
					triArea = 0;
				}
				break;
			case 5:
				for (int j = 0; j < indxData.GetSize(); j += 5)
				{
					trisVertex.GetSize() != 0 ? faceData.SetP0(trisVertex.GetSize()) : faceData.SetP0(0);
					faceData.SetCount(9);
					faceIndex.SetData(faceData);

					AddTri(vertex, indxData, trisVertex, j, 0, 1, 2);
					triArea = +CalcTriArea(vertex, indxData, j, 0, 1, 2);

					AddTri(vertex, indxData, trisVertex, j, 2, 3, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 2, 3, 0);

					AddTri(vertex, indxData, trisVertex, j, 3, 4, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 3, 4, 0);

					faceArea.SetData(triArea);
					triArea = 0;
				}
				break;
			case 6:
				for (int j = 0; j < indxData.GetSize(); j += 6)
				{
					trisVertex.GetSize() != 0 ? faceData.SetP0(trisVertex.GetSize()) : faceData.SetP0(0);
					faceData.SetCount(12);
					faceIndex.SetData(faceData);

					AddTri(vertex, indxData, trisVertex, j, 0, 1, 2);
					triArea = +CalcTriArea(vertex, indxData, j, 0, 1, 2);

					AddTri(vertex, indxData, trisVertex, j, 2, 3, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 2, 3, 0);

					AddTri(vertex, indxData, trisVertex, j, 3, 4, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 3, 4, 0);

					AddTri(vertex, indxData, trisVertex, j, 4, 5, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 4, 5, 0);

					faceArea.SetData(triArea);
					triArea = 0;
				}
				break;
			case 7:
				for (int j = 0; j < indxData.GetSize(); j += 7)
				{
					trisVertex.GetSize() != 0 ? faceData.SetP0(trisVertex.GetSize()) : faceData.SetP0(0);
					faceData.SetCount(15);
					faceIndex.SetData(faceData);

					AddTri(vertex, indxData, trisVertex, j, 0, 1, 2);
					triArea = +CalcTriArea(vertex, indxData, j, 0, 1, 2);

					AddTri(vertex, indxData, trisVertex, j, 2, 3, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 2, 3, 0);

					AddTri(vertex, indxData, trisVertex, j, 3, 4, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 3, 4, 0);

					AddTri(vertex, indxData, trisVertex, j, 4, 5, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 4, 5, 0);

					AddTri(vertex, indxData, trisVertex, j, 5, 6, 0);
					triArea = +CalcTriArea(vertex, indxData, j, 5, 6, 0);

					faceArea.SetData(triArea);
					triArea = 0;
				}
			default:
				break;
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

