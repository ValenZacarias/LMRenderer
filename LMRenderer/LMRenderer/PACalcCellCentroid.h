#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <unordered_set>
#include <random>

#include "DataVector.h"
#include "Cell.h"
#include "Face.h"

#include "PAReconstructCells.h"

using namespace std;

struct VectorHash {
	size_t operator()(const glm::vec3& v) const {
		std::hash<int> hasher;
		size_t seed = 0;
		for (int i = 0; i < 3; i++)
		{
			seed ^= hasher(v[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

class PACalcCellCentroid
{

public:
	PAReconstructCells reconstructCell;

	PACalcCellCentroid() {}

	template <class TFace, class TVertex, class TCell, class TCentroid>
	void Process(TFace& faceData, TVertex& vertexData, TCell& cellData, TCentroid& cellCentoids)
	{
		vector<int> cellFaces;
		unordered_set<glm::vec3, VectorHash> vertexSet;
		
		int p0;
		int count;
		int pointCount;

		glm::vec3 centroid(0.0f);

		for (int i = 0; i < cellData.GetSize(); i++)
		{
			reconstructCell.GetCellFaces(faceData, cellData, i, cellFaces);
			//__nop();
			for (int j = 0; j < cellFaces.size(); j++)
			{
				p0 = faceData.GetData(cellFaces[j]).GetP0();
				count = faceData.GetData(cellFaces[j]).GetCount();

				for (int k = p0; k < p0 + count; k++)
				{
					//cout << "v" << k << ", ";
					vertexSet.insert(vertexData.GetData(k));
					//__nop();
				}
			}

			pointCount = vertexSet.size();
			
			// Calculate centroid

			for (glm::vec3 v : vertexSet)
			{
				centroid += v;
			}

			centroid /= pointCount;

			cellCentoids.SetData(centroid);

			//__nop();

			//cout << "cent " << i << ": x = " << centroid.x << " // y = " << centroid.y << " // z = " << centroid.z << endl;

			centroid = glm::vec3(0.0);

			vertexSet.clear();
			cellFaces.clear();

		}

		//__nop();
	}

};

