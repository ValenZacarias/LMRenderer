#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"
#include "DataStructureBase.h"
#include "PATriangulate.h"
#include "PACalcNormals.h"
#include "DataVectorTest.h"

class VisualizationIndxElement : public VisualizationBase
{
private:
	//ESTO TIENE QUE ESTAR GENERALIZADO, NO PODEMOS ASIGNAR UN DATA ESPECIFICO A UNA VIZ
	std::shared_ptr<DataVectorTest<float>> vertexData;
	std::shared_ptr<DataVectorTest<int>> indexData;
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	PATriangulate triangulate;
	PACalcNormals calcnormals;
	Shader shader;
public:
	VisualizationIndxElement(GLuint* vao, GLuint* vbo);
	//Agregar constructor que tome unicamente un vetexData y arme el normalData
	VisualizationIndxElement(std::shared_ptr<DataVectorTest<float>> v, std::shared_ptr<DataVectorTest<int>> i);
	~VisualizationIndxElement();

	void Render(Camera* cam) override;
	int GenerateBuffers();
};