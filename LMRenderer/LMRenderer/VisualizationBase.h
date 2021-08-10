#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

struct Camera;

class VisualizationBase
{
public:
	VisualizationBase() {};
	virtual void Render(Camera* cam) {}; //Camera va a obtenerse de canvas.currentCamera
};