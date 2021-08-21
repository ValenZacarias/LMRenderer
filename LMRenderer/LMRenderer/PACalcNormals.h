#pragma once
#include <glm/glm.hpp> 
#include "DataVectorTest.h"

class PACalcNormals
{
public:
	PACalcNormals();
	DataVectorTest<float> Process(std::shared_ptr<DataVectorTest<float>> vertex, std::shared_ptr<DataVectorTest<int>> elementIndx, int faceVertexNum);
	//Calcula para triangulos, asume que el arreglo de vertices son triangulos
	DataVectorTest<float> Process(std::shared_ptr<DataVectorTest<float>> vertex);
};

