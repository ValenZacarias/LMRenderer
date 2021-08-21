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


public:
	DataVectorTest(DATATYPE type) : DataStructureBase(type) { }
	DataVectorTest(DATATYPE type, const T* rawdata, int count) : DataStructureBase(type)
	{
		for(int i = 0; i < count; i++) { data.push_back(rawdata[i]); }
	}

	void ReserveData(int count) { data.reserve(count); }
	T GetData(int i) { return data[i]; }
	auto GetFullData() { return data.data(); }
	void SetData(T d) { data.push_back(d); }
	int GetSize(){ return data.size(); }
};

