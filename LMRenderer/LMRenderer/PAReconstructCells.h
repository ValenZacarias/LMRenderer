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
		vector<int> cellFaces;

		//for (int i = 0; i < cellData.GetSize(); i += 7)
		for (int i = 0; i < cellData.GetSize(); i += 1)
		{
			GetCellFaces(faceData, cellData, i, cellFaces);
			for (int j = 0; j < cellFaces.size(); j++)
			{
				reconstructedFaceData.SetData(faceData.GetData(cellFaces[j]));
			}

			cellFaces.clear();
		}

	}

	template <class TFace, class TCell>
	void GetCellFaces(TFace& faceData, TCell& cellData, int cellIndex, vector<int>& cellFaces)
	{

		//se puede evitar
		//cellFaces.push_back(cellData.GetData(cellIndex).GetFaceIndex());

		int existingFaceIndex;
		bool existingFaceSide;
		if (cellData.GetData(cellIndex).GetSide() == false) // Right
		{
			existingFaceIndex = faceData.GetData(cellData.GetData(cellIndex).GetFaceIndex()).GetNextRIndex();
			existingFaceSide  = faceData.GetData(cellData.GetData(cellIndex).GetFaceIndex()).GetNextRSide();
		}
		else // Left
		{
			existingFaceIndex = faceData.GetData(cellData.GetData(cellIndex).GetFaceIndex()).GetNextLIndex();
			existingFaceSide = faceData.GetData(cellData.GetData(cellIndex).GetFaceIndex()).GetNextLSide();
		}

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
			
			cellFaces.push_back(existingFaceIndex);
		}

	}

};

