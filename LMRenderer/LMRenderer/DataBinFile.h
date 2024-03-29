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
	T dataAux;

public:

	DataBinFile(DATATYPE type, string name) : DataStructureBase(type)
	{ 
		filename = name;
		file.open(filename, std::ios::in | std::ios::binary);

		if (!file.is_open())
		{
			cout << name << " not found!" << endl;
			file.open(filename, ios::in | ios::binary | ios::trunc);
			cout << "file " << filename << " created" << endl;
		}
		else
		{
			cout << name << " already exists" << endl;
			
			file.seekg(0, ios::end);
			size = file.tellg() / sizeof(T);
			existingFile = true;
		}

		file.close();
	}

	bool FileExists() { return existingFile; }

	void SaveFile() { file.flush(); }

	bool CloseFile()
	{
		file.close();
		file.clear();

		if (!file.is_open())
			return true;

		return false;
	}

	bool StartWrite()
	{
		file.open(filename, std::ios::out | std::ios::binary);

		if (!file.is_open())
			cout << "Could not open " << filename << " for write" << endl;  return false;

		return true;	
	}


	bool StartRead()
	{
		file.open(filename, std::ios::in | std::ios::binary);

		if (!file.is_open())
			cout << "Could not open " << filename << " for read" << endl;  
		return false;

		return true;
	}

	void SetData(T d)
	{
		file.seekp(size * sizeof(T));
		file.write(reinterpret_cast<char*>(&d), sizeof(d));
		size++;
	}

	T& GetData(int i) 
	{ 
		file.seekg(i * sizeof(T));
		file.read(reinterpret_cast<char*>(&dataAux), sizeof(dataAux));

		//__nop();

		return dataAux;
	}

	int GetSize() { return size; }
	
	void ReserveData(int count) { }

	void GetFullData(vector<T>& buffer) 
	{ 
		file.seekg(0);

		// no podemos hacer &buffer dado que eso es un puntero a la clase, no a los datos
		//para eso usamos buffer.data() que nos da un puntero a la direccion donde almacena
		file.read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(T));
	}


};

