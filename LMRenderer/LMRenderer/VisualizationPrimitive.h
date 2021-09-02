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

class VisualizationPrimitive : public VisualizationBase
{ 
private:
	//ESTO TIENE QUE ESTAR GENERALIZADO, NO PODEMOS ASIGNAR UN DATA ESPECIFICO A UNA VIZ
	std::shared_ptr<DataVector<float>> vertexdata;
	std::shared_ptr<DataVector<float>> normaldata;
	GLuint VAO;
	GLuint VBO;
	Shader shader;
public:
	VisualizationPrimitive(GLuint* vao, GLuint* vbo);
	//Agregar constructor que tome unicamente un vetexData y arme el normalData
	VisualizationPrimitive(std::shared_ptr<DataVector<float>> v, std::shared_ptr<DataVector<float>> n);
	~VisualizationPrimitive();

	void Render(Camera* cam) override;
	int GenerateBuffers();
};