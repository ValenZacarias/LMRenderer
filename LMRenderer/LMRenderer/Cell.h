#pragma once

class Cell
{
public:
	int faceIndex;
	bool side;

	void SetFaceIndex(int fi) { faceIndex = fi; }
	void SetSide(bool s) { side = s; }
	int GetFaceIndex() { return faceIndex; }
	bool GetSide() { return side; }
};