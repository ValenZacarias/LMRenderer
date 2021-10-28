#pragma once
#include <glm/glm.hpp> 
#include "Face.h"

class PAFaceIndexTriangulate
{
public:
	PAFaceIndexTriangulate() {}

	DataVector<glm::vec3> Process(DataVector<glm::vec3>& vertex, DataVector<Face>& faces)
	{
		DataVector<glm::vec3> trisData(POINT);

		int p0Index;
		for (int i = 0; i < faces.GetSize(); i ++)
		{
			p0Index = faces.GetData(i).GetP0();
			for (int j = 0; j < faces.GetData(i).GetCount() ; j++)
			{
				trisData.SetData(vertex.GetData( p0Index + j ));
			}
		}

		return trisData;
	}

	template <typename TOut>
	void Process(TOut& trisData, DataVector<glm::vec3>& vertex, DataVector<Face>& faces)
	{
		float faceCount = faces.GetSize();

		int p0Index;
		for (int i = 0; i < faces.GetSize(); i++)
		{
			//cout << " face loading " << (float)(i / faceCount) << endl;
			p0Index = faces.GetData(i).GetP0();
			for (int j = 0; j < faces.GetData(i).GetCount(); j++)
			{
				//cout << "x = " << vertex.GetData(p0Index + j).x
				//	<< "y = " << vertex.GetData(p0Index + j).y
				//	<< "z = " << vertex.GetData(p0Index + j).z << endl;

				trisData.SetData(vertex.GetData(p0Index + j));
			}
		}
	}


private:

};

