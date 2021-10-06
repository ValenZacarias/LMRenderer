#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>

#include "DataVector.h"
#include "Cell.h"
#include "Face.h"

class PAReconstructCells
{
public:
	PAReconstructCells() {}

	template <class T>
	void Process(T& faceData, T& reconstructedFaceData, DataVector<Cell>& cellData)
	{
		int cellIndex;
		int firstFace;
		int existingFaceIndex;
		bool existingFaceSide;

		//for (int i = 0; i < cellData.GetSize(); i += 7)
		for (int i = 0; i < cellData.GetSize(); i += 1)
		{
			cellIndex = i;

			firstFace = cellData.GetData(cellIndex).GetFaceIndex();
			reconstructedFaceData.SetData(faceData.GetData(firstFace));

			existingFaceIndex = faceData.GetData(firstFace).GetNextRIndex(); // Owned face is always at right
			existingFaceSide = faceData.GetData(firstFace).GetNextRSide();
			reconstructedFaceData.SetData(faceData.GetData(existingFaceIndex));

			while (existingFaceIndex != firstFace)
			{
				if (!existingFaceSide) // right side
				{
					existingFaceSide = faceData.GetData(existingFaceIndex).GetNextRSide();
					existingFaceIndex = faceData.GetData(existingFaceIndex).GetNextRIndex();
				}
				else // left side
				{
					existingFaceSide = faceData.GetData(existingFaceIndex).GetNextLSide();
					existingFaceIndex = faceData.GetData(existingFaceIndex).GetNextLIndex();
				}
				reconstructedFaceData.SetData(faceData.GetData(existingFaceIndex));
			}


		}
	}

};

