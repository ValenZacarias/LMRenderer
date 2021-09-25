#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>

#include "DataVector.h"

class PACalcCDF
{
public:
	PACalcCDF() {}

	vector<float> Process(DataVector<float>& data)
	{
		vector<float> CDF;
		CDF.reserve(data.GetSize());

		CDF.push_back(data.GetData(0));

		for (int i = 1; i < data.GetSize(); i++)
		{
			CDF.push_back(data.GetData(i) + CDF[i - 1]);
		}

		for (int i = 0; i < CDF.size(); ++i)
			CDF[i] = CDF[i] / CDF[CDF.size() - 1];

		return CDF;
	}
	
};

