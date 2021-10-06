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


		// Process Owner data -----------
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

	

		
		// Process Bonudary data -----------
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

		// Process Neighbour data -----------
		int faceIndex;
		bool firstFaceNextSide;

		for (int i = 0; i < neighbourData.GetSize(); i++)
		{
			if (faceData.GetData(i).isBoundary())
			{
				i--;
				continue;
			}

			if (neighbourData.GetData(i) == 4) 
				__nop();

			faceIndex = i;
			cellIndex = neighbourData.GetData(i);
			
			// Existing face index is the face who is pointing the owned cell's face
			cellFirstFaceIndex = cellData.GetData(cellIndex).GetFaceIndex();
			firstFaceNextIndex = faceData.GetData(cellFirstFaceIndex).GetNextRIndex(); // cells always starts at right
			firstFaceNextSide = faceData.GetData(cellFirstFaceIndex).GetNextRSide();

			// FACE POINTED FROM CELL, NOW POINTS TO NEW FACE
			faceData.GetData(cellFirstFaceIndex).SetNextRIndex(faceIndex);
			faceData.GetData(cellFirstFaceIndex).SetNextRSide(true);

			// NEW FACE NOW POINTS WHERE EXISTING FACENEXT WAS POINTING
			faceData.GetData(faceIndex).SetNextLIndex(firstFaceNextIndex);
			faceData.GetData(faceIndex).SetNextLSide(firstFaceNextSide);

		}

		vector<int> cellFaces;
		int count;
		int firstFace;
		int existingFaceIndex;
		bool existingFaceSide;
		for (int i = 0; i < cellData.GetSize(); i++)
		{
			cellIndex = i;
			firstFace = cellData.GetData(cellIndex).GetFaceIndex();
			existingFaceIndex = faceData.GetData(firstFace).GetNextRIndex(); // Owned face is always at right
			existingFaceSide = faceData.GetData(firstFace).GetNextRSide();
			count = 1;
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
				count++;
			}
			cellFaces.push_back(count);
		}
 		__nop();
		

	}

};