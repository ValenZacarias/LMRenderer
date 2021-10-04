#pragma once

class Cell
{
public:
	int faceIndex;
	int side;

	void SetFaceIndex(int fi) { faceIndex = fi; }
	void SetSide(int s) { side = s; }
	int GetFaceIndex() { return faceIndex; }
	int GetSide() { return side; }
};