#pragma once

#include "Face.h"

class PACreateFaceData
{
public:
	PACreateFaceData() {}

	template <typename TOwner, typename TFaces>
	void Process(TOwner& ownerData, TFaces& faceData)
	{
		for (int i = 0; i < ownerData.GetSize(); i++)
		{
			Face newFace;
			faceData.SetData(newFace);
		}
	}
};