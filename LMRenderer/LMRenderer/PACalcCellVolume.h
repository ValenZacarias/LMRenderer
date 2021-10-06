#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>

#include "DataVector.h"
#include "Cell.h"
#include "Face.h"

class PACalcCellVolume
{
public:
	PACalcCellVolume() {}

	template <class T>
	void Process(T& faceData, T& reconstructedFaceData, DataVector<Cell>& cellData)
	{
		int cellFaceCount;
		int firstFaceIndex;
		int faceIndex;
		int faceSide;

		//for (int i = 0; i < cellData.GetSize(); i += 7)
		for (int i = 0; i < cellData.GetSize(); i += 1)
		{
			cellFaceCount = 0;

			firstFaceIndex = cellData.GetData(i).GetFaceIndex();
			faceIndex = firstFaceIndex;
			faceSide = cellData.GetData(i).GetSide();

			reconstructedFaceData.SetData(faceData.GetData(faceIndex));

			while (cellFaceCount < 6)
			{
				//cout << "Cell " << i << " Face " << cellFaceCount << ": " << faceIndex << endl;
				// Follow the sides of every face
				if (!faceSide) // false is right side
				{
					if (faceIndex == -1) break;
					faceSide = faceData.GetData(faceIndex).GetNextRSide();
					faceIndex = faceData.GetData(faceIndex).GetNextRIndex();
				}
				else
				{
					if (faceIndex == -1) break;
					faceSide = faceData.GetData(faceIndex).GetNextLSide();
					faceIndex = faceData.GetData(faceIndex).GetNextLIndex();
				}
				if (faceIndex == -1) break;
				reconstructedFaceData.SetData(faceData.GetData(faceIndex));
				cellFaceCount++;
			}

		}
	}

};

