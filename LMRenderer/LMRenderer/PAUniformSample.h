#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>

#include "DataVector.h"

class PAUniformSample
{
public:
	PAUniformSample() {}

	DataVector<glm::vec3> Process(DataVector<glm::vec3> vertex, float reductionFactor)
	{
		DataVector<glm::vec3> sampledData(POINT);
		

		std::random_device rd{};
		std::mt19937 engine{ rd() };
		std::uniform_int_distribution<long> dist{ 0, ( vertex.GetSize() / 3 - 1 ) };

		long rnd;
		long sampleSize = ceil(vertex.GetSize() * reductionFactor);


		for (int i = 0; i < sampleSize; i++)
		{
			rnd = dist(engine) * 3;

			if (rnd > vertex.GetSize())
			{
				cout << "OUT OF RANGE" << endl;

			}

			sampledData.SetData(vertex.GetData(rnd));
			sampledData.SetData(vertex.GetData(rnd + 1));
			sampledData.SetData(vertex.GetData(rnd + 2));
		}

		return sampledData;
	}
};

