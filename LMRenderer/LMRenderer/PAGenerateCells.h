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
		Face aux;
		Face& existingFace = aux;
		int cellIndex;
		int existingFaceIndex;

		for (int i = 0; i < ownerData.GetSize(); i++)
		{
			Face newFace;
			faceData.SetData(newFace);

			cellIndex = ownerData.GetData(i);

			if (cellData.GetData(cellIndex).GetFaceIndex() == -1)
			{
				// INITIALIZE CELL -> SET CURRENT FACE AS CELL.FACEINDEX
				cellData.GetData(cellIndex).SetFaceIndex(i);
				cellData.GetData(cellIndex).SetSide(0); //RightSide
			}
			else
			{
				// FIND LAST FACE FROM THE CELL AND CONNECT CURRENT FACE
				existingFaceIndex = cellData.GetData(cellIndex).GetFaceIndex();
				while (faceData.GetData(existingFaceIndex).GetNextRIndex() != -1)
				{
					existingFaceIndex = faceData.GetData(existingFaceIndex).GetNextRIndex();
				}
				faceData.GetData(existingFaceIndex).SetNextRIndex(i);
				faceData.GetData(existingFaceIndex).SetNextRSide(false);
			}

		}

		vector<int> cellFaces;
		int count;
		for (int i = 0; i < cellData.GetSize(); i++)
		{
			existingFaceIndex = cellData.GetData(i).GetFaceIndex();
			count = 1;
			while (faceData.GetData(existingFaceIndex).GetNextRIndex() != -1)
			{
				existingFaceIndex = faceData.GetData(existingFaceIndex).GetNextRIndex();
				count++;
			}
			cellFaces.push_back(count);
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

 		__nop();

		// Process Neighbour data -----------
		// Iterate through boundary faces, neighbour data starts at face 0 and ends on the first boundary face
		bool existingFaceSide;
		
		int neighbourCellIndex;
		int internalFaceIndex;
		int neighbourIterator = 0;

		int cellFaceIndex;
		bool cellFaceSide;
		int currentFaceIndex = -1;
		bool currentFaceSide;

		int cellFaceCount;
		bool internalFaceFreeSide;

		int cellFirstFaceIndex;

		for (int i = 0; i < neighbourData.GetSize(); i++)
		{
			if (faceData.GetData(i).isBoundary())
				break;



			internalFaceIndex = i;
			neighbourCellIndex = neighbourData.GetData(i);

			if (neighbourCellIndex == 8)
				__nop();

			cellFirstFaceIndex = cellData.GetData(neighbourCellIndex).GetFaceIndex();
			
			cellFaceCount = 1;
			currentFaceIndex = cellFirstFaceIndex;
			currentFaceSide = cellData.GetData(neighbourCellIndex).GetSide();

			// Look for the last cell in the list
			while (cellFaceCount < cellFaces[neighbourCellIndex])
			{
				// Follow the sides of every face
				if (!currentFaceSide) // false is right side
				{
					if (currentFaceIndex == -1) break;
					currentFaceSide = faceData.GetData(currentFaceIndex).GetNextRSide();
					currentFaceIndex = faceData.GetData(currentFaceIndex).GetNextRIndex();
				}
				else
				{
					if (currentFaceIndex == -1) break;
					currentFaceSide = faceData.GetData(currentFaceIndex).GetNextLSide();
					currentFaceIndex = faceData.GetData(currentFaceIndex).GetNextLIndex();
				}

				cellFaceCount++;
			}

			if (currentFaceIndex == -1)
				continue;

			if (cellFaceCount == 5)
			{
				// The internal face must connect with the first face from the neighbour cell. This face has another owner
				// and now we set the first face from the neighbour cell to close the list
				faceData.GetData(internalFaceIndex).SetNextLIndex(cellFirstFaceIndex);
				faceData.GetData(internalFaceIndex).SetNextLSide(false); // We are connecting to a face owned, so we go Rigth always
			}

			// Check if list is complete (for tetrahedra) NEEDS TO HANDLE ALL POLIHEDRA
			if (cellFaceCount == 6)
				continue;

			// Check for the free side of the current face
			if (faceData.GetData(currentFaceIndex).GetNextRIndex() == -1)
			{
				faceData.GetData(currentFaceIndex).SetNextRIndex(internalFaceIndex);

				// Check for the new inner face (the one who is going to be connected) free slot
				if (faceData.GetData(internalFaceIndex).GetNextRIndex() == -1)
				{
					faceData.GetData(currentFaceIndex).SetNextRSide(false);
					faceData.GetData(internalFaceIndex).SetNextRIndex(internalFaceIndex);
				}
				else
				{
					faceData.GetData(currentFaceIndex).SetNextRSide(true);
					faceData.GetData(internalFaceIndex).SetNextLIndex(internalFaceIndex);
				}
			}
			else
			{
				faceData.GetData(currentFaceIndex).SetNextLIndex(internalFaceIndex);
				if (faceData.GetData(internalFaceIndex).GetNextRIndex() == -1)
				{
					faceData.GetData(currentFaceIndex).SetNextLSide(false);
					faceData.GetData(internalFaceIndex).SetNextRIndex(internalFaceIndex);
				}
				else
				{
					faceData.GetData(currentFaceIndex).SetNextLSide(true);
					faceData.GetData(internalFaceIndex).SetNextLIndex(internalFaceIndex);
				}
			}

			cellFaces[neighbourCellIndex]++;

		}

		__nop();

	}

};