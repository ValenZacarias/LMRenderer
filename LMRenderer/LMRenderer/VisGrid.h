#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "shader_s.h"
#include "VisBase.h"
#include "DataStructureBase.h"
#include "DataVector.h"

class VisGrid : public VisBase
{
private:
	GLuint VAO;
	GLuint VBO;
	int lenght;
	Shader shader;
public:
	VisGrid();

	void Render(Camera* cam) override;
	int GenerateBuffers();
};