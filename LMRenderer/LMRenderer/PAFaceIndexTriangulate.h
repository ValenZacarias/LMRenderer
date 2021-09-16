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


	template <typename TVertex, typename TIndex>
	TVertex Process(TVertex& vertex, TIndex& elementIndx, int perElemIndex)
	{
		//Este metodo genera un DS de flotantes con las coordenadas de los vertices que obtiene de triangular quads
		//segun los indices de este
		TVertex trisVertex(FLOATVAL);

		int t1_0;
		int t1_1;
		int t1_2;

		int t2_0;
		int t2_1;
		int t2_2;

		//Refactorizar de forma que use el structs de Polygons
		for (int i = 0; i < elementIndx.GetSize(); i += perElemIndex) //Agarro el quad completo
		{
			//Tri 1 (3 vertex)
			// 3 * debido a la cantidad de floats por vertice
			t1_0 = elementIndx.GetData(i);
			t1_1 = elementIndx.GetData(i + 1);
			t1_2 = elementIndx.GetData(i + 2);

			trisVertex.SetData(vertex.GetData(3 * t1_0));
			trisVertex.SetData(vertex.GetData(3 * t1_0 + 1));
			trisVertex.SetData(vertex.GetData(3 * t1_0 + 2));

			trisVertex.SetData(vertex.GetData(3 * t1_1));
			trisVertex.SetData(vertex.GetData(3 * t1_1 + 1));
			trisVertex.SetData(vertex.GetData(3 * t1_1 + 2));

			trisVertex.SetData(vertex.GetData(3 * t1_2));
			trisVertex.SetData(vertex.GetData(3 * t1_2 + 1));
			trisVertex.SetData(vertex.GetData(3 * t1_2 + 2));

			t2_0 = elementIndx.GetData(i + 2);
			t2_1 = elementIndx.GetData(i + 3);
			t2_2 = elementIndx.GetData(i);

			trisVertex.SetData(vertex.GetData(3 * t2_0));
			trisVertex.SetData(vertex.GetData(3 * t2_0 + 1));
			trisVertex.SetData(vertex.GetData(3 * t2_0 + 2));

			trisVertex.SetData(vertex.GetData(3 * t2_1));
			trisVertex.SetData(vertex.GetData(3 * t2_1 + 1));
			trisVertex.SetData(vertex.GetData(3 * t2_1 + 2));

			trisVertex.SetData(vertex.GetData(3 * t2_2));
			trisVertex.SetData(vertex.GetData(3 * t2_2 + 1));
			trisVertex.SetData(vertex.GetData(3 * t2_2 + 2));
		}

		return trisVertex;
	}

	template <typename TVertex, typename TIndex>
	void Process(TVertex& vertex, TIndex& elemIndxVector, DataVector<glm::vec3>& trisVertex, DataVector<Face>& faceIndex)
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

		//DataVector<glm::vec3> trisVertex(VECTOR);
		trisVertex.ReserveData(vertexCount);

		for (int i = 3; i < elemIndxVector.size(); i++)
		{
			auto& indxData = elemIndxVector[i];

			//Hacer un _expect(vpos.getType()==PointsType) para Esperar valores mayores a 2;
			switch (i)
			{
			case 3:
				for (int j = 0; j < indxData.GetSize(); j += 3)
				{
					//tri 1
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j)));
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j + 1)));
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j + 2)));
				}
				break;
			case 4:
				for (int j = 0; j < indxData.GetSize(); j += 4)
				{
					Face faceData;
					trisVertex.GetSize() != 0 ? faceData.SetP0(trisVertex.GetSize() - 1) : faceData.SetP0(0);
					faceData.SetCount(6);
					faceIndex.SetData(faceData);

					//tri 1
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j)));
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j + 1)));
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j + 2)));

					//tri 2
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j + 2)));
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j + 3)));
					trisVertex.SetData(GetPoint(vertex, indxData.GetData(j)));
				}
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				break;
			}
		}

		//return trisVertex;
	}

private:
	glm::vec3 GetPoint(DataVector<float>& vertexData, int index)
	{
		return glm::vec3(vertexData.GetData(3 * index), vertexData.GetData(3 * index + 1), vertexData.GetData(3 * index + 2));
	}
};

