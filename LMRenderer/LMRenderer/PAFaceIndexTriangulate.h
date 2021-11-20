#pragma once
#include <glm/glm.hpp> 
#include <chrono>

#include "Face.h"

using namespace chrono;

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
		steady_clock::time_point begin = steady_clock::now();

		float faceCount = faces.GetSize();

		int p0Index;
		for (int i = 0; i < faces.GetSize(); i++)
		{
			p0Index = faces.GetData(i).GetP0();
			for (int j = 0; j < faces.GetData(i).GetCount(); j++)
			{
				trisData.SetData(vertex.GetData(p0Index + j));
			}
		}
		steady_clock::time_point end = steady_clock::now();
		cout << " Storing data in binary = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
	}

	template <typename TOut>
	void Process_Offset(TOut& trisData, DataVector<glm::vec3>& vertex, DataVector<Face>& faces, glm::vec3 offset)
	{
		steady_clock::time_point begin = steady_clock::now();

		float faceCount = faces.GetSize();

		int p0Index;
		for (int i = 0; i < faces.GetSize(); i++)
		{
			p0Index = faces.GetData(i).GetP0();
			for (int j = 0; j < faces.GetData(i).GetCount(); j++)
			{
				trisData.SetData(vertex.GetData(p0Index + j) + offset);
			}
		}
		steady_clock::time_point end = steady_clock::now();
		cout << " Storing data in binary = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
	}


private:

};

