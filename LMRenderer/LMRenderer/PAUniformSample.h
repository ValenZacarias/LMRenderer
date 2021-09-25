#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <map>

#include "DataVector.h"

class PAUniformSample
{
public:
	PAUniformSample() {}

	DataVector<glm::vec3> Process(DataVector<glm::vec3> vertex, float reductionFactor)
	{
		// TriangleSampling
		DataVector<glm::vec3> sampledData(POINT);
		
		std::random_device rd{};
		std::mt19937 engine{ rd() };
		std::uniform_int_distribution<long> dist{ 0, ( vertex.GetSize() / 6 - 1 ) };

		long rnd;
		long sampleSize = ceil(vertex.GetSize() * reductionFactor);


		for (int i = 0; i < sampleSize; i++)
		{
			//rnd = dist(engine) * 3; // Tris
			rnd = dist(engine) * 6;	  // Quads

			if (rnd > vertex.GetSize())
			{
				cout << "OUT OF RANGE" << endl;

			}

			sampledData.SetData(vertex.GetData(rnd));
			sampledData.SetData(vertex.GetData(rnd + 1));
			sampledData.SetData(vertex.GetData(rnd + 2));

			sampledData.SetData(vertex.GetData(rnd + 3));
			sampledData.SetData(vertex.GetData(rnd + 4));
			sampledData.SetData(vertex.GetData(rnd + 5));
		}

		return sampledData;
	}

	template <class T>
	void Process(T& dataSet, T& sampledSet, float reductionFactor)
	{
		//T sampledData;


		std::random_device rd{};
		std::mt19937 engine{ rd() };
		std::uniform_int_distribution<long> dist{ 0, (dataSet.GetSize() - 1) };

		long rnd;
		long sampleSize = ceil(dataSet.GetSize() * reductionFactor);
		sampledSet.ReserveData(sampleSize);

		for (int i = 0; i < sampleSize; i++)
		{
			rnd = dist(engine);
			sampledSet.SetData(dataSet.GetData(rnd));
		}
	}

	template <class T>
	map<float, int> Process_DebugHistogram(T& dataSet, T& sampledSet, float reductionFactor, DataVector<float>& faceArea)
	{
		//T sampledData;

		std::random_device rd{};
		std::mt19937 engine{ rd() };
		std::uniform_int_distribution<long> dist{ 0, (dataSet.GetSize() - 1) };

		long rnd;
		long sampleSize = ceil(dataSet.GetSize() * reductionFactor);
		sampledSet.ReserveData(sampleSize);

		// Histogram
		map<float, int> areaCountHistogram;
		float areaSize;

		for (int i = 0; i < sampleSize; i++)
		{
			rnd = dist(engine);
			sampledSet.SetData(dataSet.GetData(rnd));
			
			//areaSize = round(faceArea.GetData(rnd) * 1000000) / 1000000;
			areaSize = round(faceArea.GetData(rnd));
			
			if (areaCountHistogram.find(areaSize) != areaCountHistogram.end())
				areaCountHistogram[areaSize] += 1;
			else
				areaCountHistogram.insert(std::make_pair(areaSize, 1));
		}

		return areaCountHistogram;
	}
};

