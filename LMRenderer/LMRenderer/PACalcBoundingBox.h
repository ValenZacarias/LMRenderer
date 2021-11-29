#pragma once
#include <glm/glm.hpp> 
#include <vector> 

using namespace std;

class PACalcBoundingBox
{
public:
	PACalcBoundingBox() {}

	template <typename TVertex>
	void Process(TVertex& vertexData, vector<glm::vec3>& BBVertex )
	{
		glm::vec3 maxPos = glm::vec3(vertexData.GetData(0), vertexData.GetData(1), vertexData.GetData(2));
		glm::vec3 minPos = glm::vec3(vertexData.GetData(0), vertexData.GetData(1), vertexData.GetData(2));

		for (int i = 0; i < vertexData.GetSize(); i+=3)
		{
			// Max

			if (vertexData.GetData(i) > maxPos.x)
				maxPos.x = vertexData.GetData(i);

			if (vertexData.GetData(i + 1) > maxPos.y)
				maxPos.y = vertexData.GetData(i + 1);

			if (vertexData.GetData(i + 2) > maxPos.z)
				maxPos.z = vertexData.GetData(i + 2);

			// Min
			if (vertexData.GetData(i) < minPos.x)
				minPos.x = vertexData.GetData(i);

			if (vertexData.GetData(i + 1) < minPos.y)
				minPos.y = vertexData.GetData(i + 1);

			if (vertexData.GetData(i + 2) < minPos.z)
				minPos.z = vertexData.GetData(i + 2);
		}

		BBVertex.push_back(glm::vec3(minPos.x, minPos.y, minPos.z)); // 0
		BBVertex.push_back(glm::vec3(maxPos.x, minPos.y, minPos.z)); // 1
		BBVertex.push_back(glm::vec3(maxPos.x, maxPos.y, minPos.z)); // 2
		BBVertex.push_back(glm::vec3(minPos.x, maxPos.y, minPos.z)); // 3
		BBVertex.push_back(glm::vec3(minPos.x, minPos.y, maxPos.z)); // 4
		BBVertex.push_back(glm::vec3(maxPos.x, minPos.y, maxPos.z)); // 5
		BBVertex.push_back(glm::vec3(maxPos.x, maxPos.y, maxPos.z)); // 6
		BBVertex.push_back(glm::vec3(minPos.x, maxPos.y, maxPos.z)); // 7

		//__nop();
	}
};