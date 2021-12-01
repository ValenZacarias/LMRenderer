#pragma once
#include <glm/glm.hpp> 
#include "Face.h"
#include "Cell.h"

class PAGenerateCells
{
public:
	PAGenerateCells() {}

	template <typename TOwner, typename TNeighbour, typename TBounds, typename TFaces, typename TCells>
	void Process(TOwner& ownerData, 
				 TNeighbour& neighbourData, 
				 TBounds& boundsData, 
				 TFaces& faceData, 
				 TCells& cellData, 
				 const int cellCount)
	{
		faceData.ReserveData(ownerData.GetSize());
		cellData.ReserveData(cellCount);

		for (int i = 0; i < cellCount; i++)
		{
			Cell c;
			c.SetFaceIndex(-1);
			c.SetSide(-1);
			cellData.SetData(c);
		}


		// 1. Process Owner data -----------
		int firstFaceNextIndex;
		int cellFirstFaceIndex;
		int newFaceIndex;
		int cellIndex;

		for (int i = 0; i < ownerData.GetSize(); i++)
		{
			newFaceIndex = i;
			cellIndex = ownerData.GetData(i);

			if (cellData.GetData(cellIndex).GetFaceIndex() == -1)
			{
				// INITIALIZE CELL -> SET CURRENT FACE AS CELL.FACEINDEX
				cellData.GetData(cellIndex).SetFaceIndex(newFaceIndex);
				cellData.GetData(cellIndex).SetSide(0); //RightSide

				// CURRENT FACE RIGHT SIDE POINTS ITSELF
				faceData.GetData(newFaceIndex).SetNextRIndex(newFaceIndex);
				faceData.GetData(newFaceIndex).SetNextRSide(false);
			}
			else
			{
				// Existing face index is the face who is pointing the owned cell's face
				cellFirstFaceIndex = cellData.GetData(cellIndex).GetFaceIndex();
				firstFaceNextIndex= faceData.GetData(cellFirstFaceIndex).GetNextRIndex();

				// NEW FACE NOW POINTS WHERE EXISTING FACE WAS POINTING
				faceData.GetData(newFaceIndex).SetNextRIndex(firstFaceNextIndex);
				faceData.GetData(newFaceIndex).SetNextRSide(false);

				// FACE POINTED FROM CELL, NOW POINTS TO NEW FACE
				faceData.GetData(cellFirstFaceIndex).SetNextRIndex(newFaceIndex);
				faceData.GetData(cellFirstFaceIndex).SetNextRSide(false);
			}
		}

		// 2. Process Bonudary data -----------
		int nFaces;
		int startFace;
		int firstBoundaryFace;

		for (int i = 0; i < boundsData.GetSize(); i += 2)
		{
			nFaces = boundsData.GetData(i);
			startFace = boundsData.GetData(i + 1);

			if (i == 0)
				firstBoundaryFace = startFace;

			for (int j = startFace; j < startFace + nFaces; j++)
			{
				faceData.GetData(j).SetBoundary(true);
			}
		}

		// 3. Process Neighbour data -----------
		int faceIndex;
		bool firstFaceNextSide;

		for (int i = 0; i < neighbourData.GetSize(); i++)
		{
			if (faceData.GetData(i).isBoundary()) { i--; continue; }

			newFaceIndex = i;
			//Face& newFace = faceData.GetData(newFaceIndex);
			cellIndex = neighbourData.GetData(i);
			//Cell& cell = cellData.GetData(cellIndex);

			if (cellData.GetData(cellIndex).GetFaceIndex() == -1)
			{
				// INITIALIZE CELL -> SET CURRENT FACE AS CELL.FACEINDEX
				cellData.GetData(cellIndex).SetFaceIndex(newFaceIndex);
				cellData.GetData(cellIndex).SetSide(true); //LeftSide

				// CURRENT FACE LEFT SIDE POINTS ITSELF
				faceData.GetData(newFaceIndex).SetNextLIndex(newFaceIndex);
				faceData.GetData(newFaceIndex).SetNextLSide(true);
			}
			else
			{
				cellFirstFaceIndex = cellData.GetData(cellIndex).GetFaceIndex();
				//Face& cellFace = faceData.GetData(cellFirstFaceIndex);

				if (cellData.GetData(cellIndex).GetSide() == false)
				{
					firstFaceNextIndex = faceData.GetData(cellFirstFaceIndex).GetNextRIndex();
					firstFaceNextSide = faceData.GetData(cellFirstFaceIndex).GetNextRSide();
					//Face& cellFaceNext = faceData.GetData(firstFaceNextIndex);

					// FACE POINTED FROM CELL, NOW POINTS TO NEW FACE
					faceData.GetData(cellFirstFaceIndex).SetNextRIndex(newFaceIndex);
					faceData.GetData(cellFirstFaceIndex).SetNextRSide(true);

					// NEW FACE NOW POINTS WHERE EXISTING FACENEXT WAS POINTING
					faceData.GetData(newFaceIndex).SetNextLIndex(firstFaceNextIndex);
					faceData.GetData(newFaceIndex).SetNextLSide(firstFaceNextSide);
				}
				else
				{
					firstFaceNextIndex = faceData.GetData(cellFirstFaceIndex).GetNextLIndex();
					firstFaceNextSide = faceData.GetData(cellFirstFaceIndex).GetNextLSide();
					//Face& cellFaceNext = faceData.GetData(firstFaceNextIndex);

					// FACE POINTED FROM CELL, NOW POINTS TO NEW FACE
					faceData.GetData(cellFirstFaceIndex).SetNextLIndex(newFaceIndex);
					faceData.GetData(cellFirstFaceIndex).SetNextLSide(true);

					// NEW FACE NOW POINTS WHERE EXISTING FACENEXT WAS POINTING
					faceData.GetData(newFaceIndex).SetNextLIndex(firstFaceNextIndex);
					faceData.GetData(newFaceIndex).SetNextLSide(firstFaceNextSide);
				}
			}
		}

		__nop();
	}

};