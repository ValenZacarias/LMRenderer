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

	template <class TFace, class TCell>
	void Process(TFace& faceData, TFace& reconstructedFaceData, TCell& cellData)
	{
		//int cellIndex;
		//int firstFace;
		//int existingFaceIndex;
		//bool existingFaceSide;

		vector<int> cellFaces;

		//for (int i = 0; i < cellData.GetSize(); i += 7)
		for (int i = 0; i < cellData.GetSize(); i += 1)
		{
			GetCellFaces(faceData, cellData, i, cellFaces);
			for (int j = 1; j < cellFaces.size(); j++)
			{
				reconstructedFaceData.SetData(faceData.GetData(cellFaces[j]));
			}
			cellFaces.clear();

			//cellIndex = i;

			//firstFace = cellData.GetData(cellIndex).GetFaceIndex();
			//reconstructedFaceData.SetData(faceData.GetData(firstFace));

			//existingFaceIndex = faceData.GetData(firstFace).GetNextRIndex(); // Owned face is always at right
			//existingFaceSide = faceData.GetData(firstFace).GetNextRSide();
			//reconstructedFaceData.SetData(faceData.GetData(existingFaceIndex));

			//while (existingFaceIndex != firstFace)
			//{
			//	if (!existingFaceSide) // right side
			//	{
			//		existingFaceSide = faceData.GetData(existingFaceIndex).GetNextRSide();
			//		existingFaceIndex = faceData.GetData(existingFaceIndex).GetNextRIndex();
			//	}
			//	else // left side
			//	{
			//		existingFaceSide = faceData.GetData(existingFaceIndex).GetNextLSide();
			//		existingFaceIndex = faceData.GetData(existingFaceIndex).GetNextLIndex();
			//	}
			//	reconstructedFaceData.SetData(faceData.GetData(existingFaceIndex));
			//}
		}

	}

	template <class TFace, class TCell>
	void GetCellFaces(TFace& faceData, TCell& cellData, int cellIndex, vector<int>& cellFaces)
	{

		//firstFace = cellData.GetData(cellIndex).GetFaceIndex();
		//reconstructedFaceData.SetData(faceData.GetData(firstFace));
		cellFaces.push_back(cellData.GetData(cellIndex).GetFaceIndex());

		int existingFaceIndex = faceData.GetData(cellData.GetData(cellIndex).GetFaceIndex()).GetNextRIndex(); // Owned face is always at right
		bool existingFaceSide = faceData.GetData(cellData.GetData(cellIndex).GetFaceIndex()).GetNextRSide();
		//reconstructedFaceData.SetData(faceData.GetData(existingFaceIndex));
		cellFaces.push_back(existingFaceIndex);

		while (existingFaceIndex != cellData.GetData(cellIndex).GetFaceIndex())
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
			//reconstructedFaceData.SetData(faceData.GetData(existingFaceIndex));
			
			cellFaces.push_back(existingFaceIndex);
		}
	}

};

