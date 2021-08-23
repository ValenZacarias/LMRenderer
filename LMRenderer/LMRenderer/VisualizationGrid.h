#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"
#include "DataStructureBase.h"
#include "DataVectorTest.h"

class VisualizationGrid : public VisualizationBase
{
private:
	//ESTO TIENE QUE ESTAR GENERALIZADO, NO PODEMOS ASIGNAR UN DATA ESPECIFICO A UNA VIZ
	//std::shared_ptr<DataVectorTest<float>> vertexdata;
	//std::shared_ptr<DataVectorTest<float>> normaldata;
	GLuint VAO;
	GLuint VBO;
	int lenght;
	Shader shader;
public:
	VisualizationGrid();

	void Render(Camera* cam) override;
	int GenerateBuffers();
};