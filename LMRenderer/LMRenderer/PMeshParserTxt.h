#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

#include "DataVector.h"

 
class PMeshPareserTxt
{
public:
	PMeshPareserTxt() {};

	vector<DataVector<int>> ParseFaces(string filename)
	{

		vector<DataVector<int>> dataIndexVector = {};
		for (int i = 0; i < 12; i++)
			dataIndexVector.push_back(DataVector<int>(INTVAL));

		if (dataIndexVector[0].GetSize() == 0)
		{
			cout << "0 = VACIO" << endl;
		}

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

		int polygonVertices;
		string v;
		size_t aux1;
		string value;

		////auto DataIndex = std::make_shared<DataVector<int>>(INTVAL);

		while (datastr.find(")\n)") != string::npos)
		{
			pos1 = datastr.find("(");
			pos2 = datastr.find(")");
			polygonVertices = stoi(datastr.substr(0, pos1));// 4 (en el caso de un quad)

			//v = istringstream(datastr.substr(pos1+1, pos2)); //(8 12 13 9) Asi queda v
			v = datastr.substr(pos1 + 1, pos2); // 8 12 13 9)\n Asi queda v
			aux1 = v.find(" ");

			do
			{
				value = v.substr(0, aux1);
				dataIndexVector[polygonVertices].SetData(stoi(value));
				v = v.erase(0, aux1 + 1);
				aux1 = v.find(" ");
			} while (aux1 != string::npos);

			v.pop_back(); // elimino el ultimo \n
			v.pop_back(); // elimino el ultimo parentesis
			value = v; //queda el ultimo numero dentro de v
			dataIndexVector[polygonVertices].SetData(stoi(value));

			//std::cout << "CANT INDX: " << elemIndxs << std::endl;
			//std::cout << v.str() << std::endl;
			datastr.erase(pos1, pos2 + 2);
		}
		
		return dataIndexVector;
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

		std::cout << "VRTX COUNT: " << countElem << std::endl;

		datastr.erase(0, countElem.size() + 2);
		//std::cout << datastr << std::endl;

		string elemIndxs;
		string v;
		size_t aux1;
		string value;

		auto DataIndex = std::make_shared<DataVector<float>>(FLOATVAL);

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
				DataIndex->SetData(stof(value) * 10.0f);
				v = v.erase(0, aux1 + 1);
				aux1 = v.find(" ");
			} while (aux1 != string::npos);

			v.pop_back(); // elimino el ultimo parentesis
			value = v; //queda el ultimo numero dentro de v
			DataIndex->SetData(stof(value) * 10.0f);

			//std::cout << "CANT INDX: " << elemIndxs << std::endl;
			//std::cout << v.str() << std::endl;
			datastr.erase(pos1, pos2 + 2);
		}

		return DataIndex;
	}


private:

};