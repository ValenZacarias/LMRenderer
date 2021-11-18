#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <chrono>

#include <glm/glm.hpp>

#include "DataVector.h"
#include "Globals.h"
#include <cassert>;

#include <iostream>


using namespace std;
using namespace chrono;

struct Face;
 
class PMeshParser
{
public:
	PMeshParser() {};

	std::shared_ptr<vector<DataVector<int>>> ParseFaces(string filename)
	{
		steady_clock::time_point begin = steady_clock::now();

		vector<DataVector<int>> dataIndexVector = {};

		for (int i = 0; i < 12; i++)
			dataIndexVector.push_back(DataVector<int>(INTVAL));

		//FILE PARSING
		ifstream in(filename);

		assert(in);

		//__M_Assert(in == true, "MyFunction: requires non-null argument");

		string currentLine;
		
		int totalFaces;
		int faceVertices;
		int charPtr;
		
		string index;
		string endOfIndices = ")";
		string indexSeparation = " ";
		
		bool parsingFaces = false;
		int faceIndex = 0; //index of the face in the faces.txt array, used when triangulating
		
		while (std::getline(in, currentLine))
		{
			// Parse face amount
			if (!parsingFaces)
			{
				if (isdigit(currentLine[0]))
				{
					totalFaces = stoi(currentLine);
					parsingFaces = true;
					continue;
				}
			}

			// Parse face
			if (parsingFaces)
			{
				if (isdigit(currentLine[0]))
				{
					faceVertices = (int)currentLine[0] - 48;
					charPtr = 2;

					dataIndexVector[faceVertices].SetData(faceIndex);

					while (currentLine[charPtr] != endOfIndices[0])
					{
						index.push_back(currentLine[charPtr]);

						if (currentLine[charPtr] == indexSeparation[0])
						{
							dataIndexVector[faceVertices].SetData(stoi(index));
							index.clear();
						}

						charPtr++;
					}

					dataIndexVector[faceVertices].SetData(stoi(index));
					index.clear();
					faceIndex++;
					continue;
				}
			}
		}

		steady_clock::time_point end = steady_clock::now();

		cout << filename << " Parsing = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
		in.close();
		auto result = std::make_shared<vector<DataVector<int>>>(dataIndexVector);
		return result;
	}


	std::shared_ptr<DataVector<float>> ParsePoints(string filename)
	{
		steady_clock::time_point begin = steady_clock::now();

		auto dataPoints = std::make_shared<DataVector<float>>(FLOATVAL);

		//FILE PARSING
		ifstream in(filename);
		string currentLine;

		int totalPoints;
		int charPtr;

		string pointCoord;
		string startOfCoords = "(";
		string endOfCoords = ")";
		string minusChar = "-";
		string pointSeparation = " ";

		bool test = isdigit(minusChar[0]);

		bool parsingPoints = false;

		while (std::getline(in, currentLine))
		{
			// Parse point amount
			if (!parsingPoints)
			{
				if (isdigit(currentLine[0]))
				{
					totalPoints = stoi(currentLine);
					dataPoints->ReserveData(totalPoints);
					continue;
				}
				if (currentLine[0] == startOfCoords[0])
				{
					parsingPoints = true;
					continue;
				}
			}

			// Parse point
			if (parsingPoints)
			{
				if (currentLine[0] == startOfCoords[0] || currentLine[0] == minusChar[0])
				{
					charPtr = 1;
					while (currentLine[charPtr] != endOfCoords[0])
					{
						pointCoord.push_back(currentLine[charPtr]);

						if (currentLine[charPtr] == pointSeparation[0])
						{
							dataPoints->SetData(stof(pointCoord));
							pointCoord.clear();
						}

						charPtr++;
					}

					dataPoints->SetData(stof(pointCoord));
					pointCoord.clear();
					continue;
				}
			}

		}

		steady_clock::time_point end = steady_clock::now();

		cout << filename << " Parsing = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
		in.close();
		return dataPoints;
	}

	int ParseCellCount(string filename)
	{
		steady_clock::time_point begin = steady_clock::now();

		//FILE PARSING
		ifstream in(filename);
		string currentLine;
		string endOfCellCount = "n";

		int lineIndex = 0;
		int cellCount;

		while (std::getline(in, currentLine))
		{

			if (lineIndex == 12)
			{
				std::size_t auxIt = currentLine.find("nCells:") + 7;
				string cellCountData;
				while (currentLine[auxIt] != endOfCellCount[0])
				{
					if(isdigit(currentLine[auxIt]))
						cellCountData.push_back(currentLine[auxIt]);
					auxIt++;
				}

				cellCount = stoi(cellCountData);
				return cellCount;
			}
			else
				lineIndex++;
		}
		return -1;
	}

	std::shared_ptr<DataVector<int>> ParseCells(string filename)
	{
		steady_clock::time_point begin = steady_clock::now();

		auto dataCells = std::make_shared<DataVector<int>>(INTVAL);

		//FILE PARSING
		ifstream in(filename);
		string currentLine;

		int totalDataIndices;
		int charPtr;

		string startOfData = "(";
		string endOfData = ")";

		bool parsingData = false;

		while (std::getline(in, currentLine))
		{
			// Parse point amount
			if (!parsingData)
			{
				if (isdigit(currentLine[0]))
				{
					totalDataIndices = stoi(currentLine);
					dataCells->ReserveData(totalDataIndices);
					continue;
				}

				if (currentLine[0] == startOfData[0])
				{
					parsingData = true;
					continue;
				}
			}

			// Parse point
			if (parsingData)
			{
				if (currentLine[0] != endOfData[0])
				{
					dataCells->SetData(stoi(currentLine));
					continue;
				}
				else
					break;
			}

		}

		steady_clock::time_point end = steady_clock::now();

		cout << filename << " Parsing = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
		in.close();
		return dataCells;
	}

	std::shared_ptr<DataVector<int>> ParseBoundary(string filename)
	{
		steady_clock::time_point begin = steady_clock::now();

		auto boundaryData = std::make_shared<DataVector<int>>(BOUNDS);

		//FILE PARSING
		ifstream in(filename);
		string currentLine;

		string startOfBoundaryData = "(";
		string startOfDataBlock = "{";
		string endOfBoundaryData = ")";
		string endOfNumber = ";";

		int currentChar;
		bool parsingData = false;

		string number;
		string nFacesStr = "nFaces";
		string startFace = "startFace";

		while (std::getline(in, currentLine))
		{
			if (currentLine[0] == startOfBoundaryData[0])
			{
				parsingData = true;
				continue;
			}

			if (parsingData)
			{
				if (currentLine[0] == endOfBoundaryData[0])
					break;

				// avoid "{"
				if (currentLine[4] == startOfDataBlock[0])
					continue;
				
				if (currentLine.find(nFacesStr) != std::string::npos || currentLine.find(startFace) != std::string::npos)
				{
					currentChar = 24;
					while (currentLine[currentChar] != endOfNumber[0])
					{
						number.push_back(currentLine[currentChar]);
						currentChar++;
					}
					boundaryData->SetData(stoi(number));
					number.clear();
					continue;
				}

			}
			
		}
		in.close();
		return boundaryData;
	}


private:

};