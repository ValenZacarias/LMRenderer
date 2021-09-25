#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <map>

#include "DataVector.h"

class PAInvTransformSample
{
public:
	PAInvTransformSample() {}

	template <class T>
	void Process(vector<float>& CDF, T& dataSet, T& sampledSet, float reductionFactor)
	{
		std::random_device rd{};
		std::mt19937 engine{ rd() };
		std::uniform_real_distribution<float> dist{ 0.0, 1.0 };

		int sampleSize = ceil(dataSet.GetSize() * reductionFactor);
		sampledSet.ReserveData(sampleSize);

		float rnd;

		std::vector<float>::iterator it = CDF.begin();
		std::vector<float>::iterator itNext = CDF.begin();
		int sampledFaceIndex;

		for (int i = 0; i < sampleSize; i++)
		{
			rnd = dist(engine);

			if (rnd <= *CDF.begin())
				sampledSet.SetData(dataSet.GetData(0));
			else
			{
				it = upper_bound(CDF.begin(), CDF.end(), rnd);
				itNext = it--;
				
				if (*itNext > rnd)
				{
					sampledFaceIndex = it - CDF.begin();
					sampledSet.SetData(dataSet.GetData(sampledFaceIndex));
				}
				else
				{
					sampledFaceIndex = itNext - CDF.begin();
					sampledSet.SetData(dataSet.GetData(sampledFaceIndex));
				}
			}
		}


		/*for (int i = 0; i < sampleSize; i++)
		{
			rnd = dist(engine);

			if (rnd <= CDF[0]/normFactor)
				sampledSet.SetData(dataSet.GetData(0));
			else
			{
				for (int j = 1; j < CDF.size(); j++)
				{
					lowBound = CDF[j - 1] / normFactor;
					upBound = CDF[j] / normFactor;
					if ( lowBound < rnd && rnd < upBound)
						sampledSet.SetData(dataSet.GetData(j));
				}
			}
		}*/
	}

	template <class T>
	map<float, int> Process_DebugHistogram(DataVector<float>& CDF, T& dataSet, T& sampledSet, float reductionFactor, DataVector<float>& faceArea)
	{
		//T sampledData;

		std::random_device rd{};
		std::mt19937 engine{ rd() };
		std::uniform_real_distribution<float> dist{ 0.0, 1.0 };

		int sampleSize = ceil(dataSet.GetSize() * reductionFactor);
		sampledSet.ReserveData(sampleSize);

		float normFactor = CDF.GetData(CDF.GetSize() - 1);
		float lowBound;
		float upBound;
		float rnd;

		// Histogram
		map<float, int> areaCountHistogram;
		float areaSize;
		int selectedIndex;
		for (int i = 0; i < sampleSize; i++)
		{
			rnd = dist(engine);

			if (rnd <= CDF.GetData(0) / normFactor)
			{
				sampledSet.SetData(dataSet.GetData(0));
				//areaSize = round(faceArea.GetData(0) * 1000000) / 1000000;
				areaSize = round(faceArea.GetData(0));

				if (areaCountHistogram.find(areaSize) != areaCountHistogram.end())
					areaCountHistogram[areaSize]++;
				else
					areaCountHistogram.insert(std::make_pair(areaSize, 1));
			}
			else
			{
				for (int j = 1; j < CDF.GetSize(); j++)
				{
					lowBound = CDF.GetData(j - 1) / normFactor;
					upBound = CDF.GetData(j) / normFactor;
					if (lowBound < rnd && rnd < upBound)
					{
						sampledSet.SetData(dataSet.GetData(j));
						selectedIndex = j;
						break;
					}


				}
				//areaSize = round(faceArea.GetData(j) * 1000000) / 1000000;
				areaSize = round(faceArea.GetData(selectedIndex));

				if (areaCountHistogram.find(areaSize) != areaCountHistogram.end())
					areaCountHistogram[areaSize]++;
				else
					areaCountHistogram.insert(std::make_pair(areaSize, 1));
			}

			
		}

		

		for (int i = 0; i < sampleSize; i++)
		{
			rnd = dist(engine);
			sampledSet.SetData(dataSet.GetData(rnd));

			
		}

		return areaCountHistogram;
	}

};

