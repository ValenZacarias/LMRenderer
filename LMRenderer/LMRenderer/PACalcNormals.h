#pragma once
#include <glm/glm.hpp>
class PACalcNormals
{
public:
	PACalcNormals() {}

	template <class TVertex>
	DataVector<glm::vec3> ProcessVec3(TVertex& vertex)
	{
		DataVector<glm::vec3> normalData(NORMAL);

		glm::vec3 normal;
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;

		for (int i = 0; i < vertex.GetSize(); i+=3)
		{
			v0 = vertex.GetData(i);
			v1 = vertex.GetData(i + 1);
			v2 = vertex.GetData(i + 2);

			normal = glm::cross(v1 - v0, v2 - v0);

			normalData.SetData(normal);
			normalData.SetData(normal);
			normalData.SetData(normal);
		}
		
		return normalData;
	}


	template <class TVertex>
	TVertex Process(TVertex &vertex)
	{
		//Calcula normales para triangulos, asume que el arreglo de vertices son triangulos
		TVertex normalData(FLOATVAL);

		glm::vec3 p1;
		glm::vec3 p2;
		glm::vec3 p3;
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 normal;

		for (int i = 0; i < vertex.GetSize(); i += 9)
		{
			p1 = glm::vec3(vertex.GetData(i),		vertex.GetData(i + 1), vertex.GetData(i + 2));
			p2 = glm::vec3(vertex.GetData(i + 3),	vertex.GetData(i + 4), vertex.GetData(i + 5));
			p3 = glm::vec3(vertex.GetData(i + 6),	vertex.GetData(i + 7), vertex.GetData(i + 8));

			v1 = p2 - p1;
			v2 = p3 - p1;

			normal = glm::cross(v1, v2);

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
};

