#pragma once

class Face
{
public:
	int p0;
	int pCount;
	
	int fNextR;
	bool fNextRSide;
	int fNextL;
	bool fNextLSide;

	bool boundary;

	Face()
	{
		p0 = -1;
		pCount = -1;
		fNextR = -1;
		fNextRSide = 0;
		fNextL = -1;
		fNextLSide = 0;
		boundary = false;
	}

	void SetP0(int p){ p0 = p; }
	int GetP0() { return p0; }
	void SetCount(int c) { pCount = c; }
	int GetCount() { return pCount; }

	void SetNextRIndex(int i) { fNextR = i; }
	int GetNextRIndex() { return fNextR; }
	void SetNextRSide(bool i) { fNextRSide = i; }
	bool GetNextRSide() { return fNextRSide; }

	void SetNextLIndex(int i) { fNextL = i; }
	int GetNextLIndex() { return fNextL; }
	void SetNextLSide(bool i) { fNextLSide = i; }
	bool GetNextLSide() { return fNextLSide; }

	void SetBoundary(bool i) { boundary = i; }
	bool isBoundary() { return boundary; }


};