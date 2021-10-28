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
	string filename;
	fstream file;
	bool existingFile = false;
	int size = 0;

public:
	DataBinFile(DATATYPE type, string name) : DataStructureBase(type)
	{ 
		filename = name;
		file.open(filename, std::ios::out | std::ios::binary);

		if (!file.is_open())
		{
			cout << name << " not found!" << endl;
			file.open(filename, ios::out | ios::binary | ios::trunc);

			if (!file.is_open())
				cout << "CANT CREATE FILE" << endl;
		}

	}

	bool FileExists() { return existingFile; }

	void SaveFile()
	{
		file.flush();
	}

	bool EndWrite()
	{
		file.close();

		if (!file.is_open())
			return true;

		return false;
	}

	bool StartRead()
	{
		file.open(filename, std::ios::in | std::ios::binary);

		if (!file.is_open())
			cout << "Could not open " << filename << " for read" << endl;  return false;

		return true;	
	}

	void ReserveData(int count) { }

	T& GetData(int i) 
	{ 
		T dataAux;

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
		file.seekp(size * sizeof(T));
		file.write(reinterpret_cast<char*>(&d), sizeof(d));
		size++;
	}

	int GetSize() 
	{ 
		return size;
	}

};

