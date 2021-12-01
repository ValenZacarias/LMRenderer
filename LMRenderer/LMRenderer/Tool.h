#pragma once
#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp> 

enum MouseState { PRESS, RELEASE, NONE };

class Tool
{
protected:
public:
	

	Tool()
	{
		std::cout << "Tool instance created" << std::endl;
	}
	virtual ~Tool() {};
	virtual void OnScroll(double yoffset) {};
	virtual void OnDrag(MouseState state) {};
	virtual void OnMouseMove(double xpos, double ypos) {};
	virtual void OnKeyPress(std::string key) {};
};