#pragma once
#include "DataStructureBase.h"
#include "Globals.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp> 
#include "Debug.h"
using namespace std;

template <class T> class DataVector : public DataStructureBase
{
private:

	vector<T> data;

public:

	DataVector(DATATYPE type) : DataStructureBase(type) { }
	~DataVector() { data.clear(); }

	void ReserveData(int count) { _expects(count >= 0); data.reserve(count); }
	T& GetData(int i) { _expects(i >= 0); _expects(i < data.size()); return data[i]; } // hacer un expect >= 0 
	auto GetFullData() { return data.data(); }
	void SetData(T d) { data.push_back(d); }
	int GetSize(){ return data.size(); }
	
	void Clear(){ data.clear(); }
};

