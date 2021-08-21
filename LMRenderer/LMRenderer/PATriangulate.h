#pragma once
#include <glm/glm.hpp> 
#include "DataVectorTest.h"

class PATriangulate
{
public:
	PATriangulate();
	DataVectorTest<float> Process(std::shared_ptr<DataVectorTest<int>> elementIndx, int elemVertex);
	DataVectorTest<float> Process(std::shared_ptr<DataVectorTest<float>> vertex,
		std::shared_ptr<DataVectorTest<int>> elementIndx,
		int perElemIndex);
};

