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
		string currentLine;
		
		int totalFaces;
		int faceVertices;
		int charPtr;
		
		string index;
		string endOfIndices = ")";
		string indexSeparation = " ";
		
		bool parsingFaces = false;
		
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
					continue;
					__nop();
				}
			}
		}

		steady_clock::time_point end = steady_clock::now();

		cout << "Face Parsing process = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
		
		auto result = std::make_shared<vector<DataVector<int>>>(dataIndexVector);
		return result;
	}

	shared_ptr<DataVector<int>> ParseFaces_Quads(string filename)
	{

		//FILE PARSING
		std::ifstream in(filename);
		std::stringstream buffer;
		buffer << in.rdbuf();
		std::string datastr = buffer.str();

		size_t pos1 = 0;
		size_t pos2;

		pos1 = datastr.find("\n\n");
		datastr = datastr.substr(pos1);
		datastr.erase(0, 3); // Con esto ya estamos en la seccion de datos parados en el numero que indica la cantidad de elementos
		pos1 = 0;
		pos2 = datastr.find("(");
		string countElem = datastr.substr(pos1, pos2);

		std::cout << "INDX COUNT: " << countElem << std::endl;

		datastr.erase(0, countElem.size() + 2);
		//std::cout << datastr << std::endl;

		string elemIndxs;
		string v;
		size_t aux1;
		string value;

		auto DataIndex = std::make_shared<DataVector<int>>(INTVAL);

		while (datastr.find(")\n)") != string::npos)
		{
			pos1 = datastr.find("(");
			pos2 = datastr.find(")");
			elemIndxs = datastr.substr(0, pos1);// 4 (en el caso de un quad)
			//v = istringstream(datastr.substr(pos1+1, pos2)); //(8 12 13 9) Asi queda v
			v = datastr.substr(pos1 + 1, pos2); // 8 12 13 9)\n Asi queda v
			aux1 = v.find(" ");

			do
			{
				value = v.substr(0, aux1);
				DataIndex->SetData(stoi(value));
				v = v.erase(0, aux1 + 1);
				aux1 = v.find(" ");
			} while (aux1 != string::npos);

			v.pop_back(); // elimino el ultimo \n
			v.pop_back(); // elimino el ultimo parentesis
			value = v; //queda el ultimo numero dentro de v
			DataIndex->SetData(stoi(value));

			//std::cout << "CANT INDX: " << elemIndxs << std::endl;
			//std::cout << v.str() << std::endl;
			datastr.erase(pos1, pos2 + 2);
		}

		return DataIndex;
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
					__nop();
				}
			}

		}

		steady_clock::time_point end = steady_clock::now();

		cout << "Point Parsing process = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;

		return dataPoints;
	}


private:

};