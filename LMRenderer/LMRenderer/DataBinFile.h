#pragma once
#include "DataStructureBase.h"
#include "Globals.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <iterator>
#include <glm/glm.hpp> 
#include "Debug.h"
using namespace std;

template <class T> class DataBinFile : public DataStructureBase
{
private:
	string filename = "test.bin"; // como puedo hacer para pasarle por parametro al nombre?
	int testWrite = 74;
	std::fstream file;
	int size = 0;

public:
	DataBinFile(DATATYPE type) : DataStructureBase(type)
	{ 
		file = std::fstream(filename, std::ios::in | std::ios::out | std::ios::binary);
	}

	void ReserveData(int count) { }

	T& GetData(int i) 
	{ 
		T dataAux;

		if (!file.is_open())
		{
			cout << "FILE DONT EXISTS" << endl;
			return dataAux;
		}

		file.seekg(i * sizeof(T));
		file.read(reinterpret_cast<char*>(&dataAux), sizeof(dataAux));

		__nop();

		return dataAux;
	}

	auto GetFullData() 
	{ 
		return data.data(); 
	}

	void SetData(T d) 
	{ 
		if (!file.is_open())
		{
			cout << "FILE DONT EXISTS" << endl;
			return;
		}

		file.seekp(size * sizeof(T));
		file.write(reinterpret_cast<char*>(&d), sizeof(d));
		size++;
	}

	int GetSize() 
	{ 
		return size;
	}

};

