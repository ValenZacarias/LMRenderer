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

class VisualizationPrimitive : public VisualizationBase
{ 
private:
	std::shared_ptr<DataVectorTest<float>> vertexdata;
	std::shared_ptr<DataVectorTest<float>> normaldata;
	GLuint VAO;
	GLuint VBO;
	Shader shader;
public:
	VisualizationPrimitive(GLuint* vao, GLuint* vbo);
	//Agregar constructor que tome unicamente un vetexData y arme el normalData
	VisualizationPrimitive(std::shared_ptr<DataVectorTest<float>> v, std::shared_ptr<DataVectorTest<float>> n);

	void Render(Camera* cam) override;
	int GenerateBuffers();
};