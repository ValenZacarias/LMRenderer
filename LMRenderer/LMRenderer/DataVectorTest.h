#pragma once
#include "DataStructureBase.h"
#include "Globals.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp> 
using namespace std;

template <class T> class DataVectorTest : public DataStructureBase
{
private:

	vector<T> data;
	int length;

public:

	DataVectorTest(DATATYPE type, const T* rawdata, int count) : DataStructureBase(type)
	{
		for(int i = 0; i < count; i++) { data.push_back(rawdata[i]); }
		length = count;
	}

	T GetData(int i) { return data[i]; }
	int GetSize(){ return length; }
};

