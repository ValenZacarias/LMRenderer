#pragma once

class Face
{
public:
	int p0;
	int pCount;

	void SetP0(int p){ p0 = p; }
	void SetCount(int c) { pCount = c; }
	int GetP0() { return p0; }
	int GetCount() { return pCount; }
};