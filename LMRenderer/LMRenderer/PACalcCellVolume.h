#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>

#include "DataVector.h"
#include "Cell.h"
#include "Face.h"

#include "PAReconstructCells.h"

class PACalcCellVolume
{
public:
	PAReconstructCells reconstructCell;

	PACalcCellVolume() {}

	template <class TFace, class TCell, class TVertex, class TCentroid>
	void Process(TFace& faceData, TVertex& vertexData, TCell& cellData, TCentroid& cellCentoids, DataVector<float>& cellVolumes)
	{

		vector<int> cellFaces;
		int p0;
		int count;

		glm::vec3 centroid(0.0f);
		glm::vec3 qi(0.0f);
		glm::vec3 qj(0.0f);
		glm::vec3 qk(0.0f);

		float vol = 0;

		for (int i = 0; i < cellData.GetSize(); i++)
		{
			reconstructCell.GetCellFaces(faceData, cellData, i, cellFaces);
			__nop();

			for (int j = 0; j < cellFaces.size(); j++)
			{
				p0 = faceData.GetData(cellFaces[j]).GetP0();
				count = faceData.GetData(cellFaces[j]).GetCount();

				centroid = cellCentoids.GetData(j);

				for (int k = p0; k < p0 + count; k+=3)
				{
					qi = vertexData.GetData(k);
					qj = vertexData.GetData(k + 1);
					qk = vertexData.GetData(k + 2);

					vol += glm::dot((qi - centroid), glm::cross((qj - centroid), (qk - centroid)));
					__nop();
				}

			}
			cellFaces.clear();
			cellVolumes.SetData(vol);
			vol = 0;
		}
		__nop();
	}

};

