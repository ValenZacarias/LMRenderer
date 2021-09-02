#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"
#include "DataStructureBase.h"
#include "DataVector.h"

class VisualizationGrid : public VisualizationBase
{
private:
	GLuint VAO;
	GLuint VBO;
	int lenght;
	Shader shader;
public:
	VisualizationGrid();

	void Render(Camera* cam) override;
	int GenerateBuffers();
};