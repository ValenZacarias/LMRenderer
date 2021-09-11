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

class VisPrimitive : public VisBase
{ 
private:
	//ESTO TIENE QUE ESTAR GENERALIZADO, NO PODEMOS ASIGNAR UN DATA ESPECIFICO A UNA VIZ
	std::shared_ptr<DataVector<float>> vertexdata;
	std::shared_ptr<DataVector<float>> normaldata;
	GLuint VAO;
	GLuint VBO;
	Shader shader;
public:
	VisPrimitive(GLuint* vao, GLuint* vbo);
	//Agregar constructor que tome unicamente un vetexData y arme el normalData
	VisPrimitive(std::shared_ptr<DataVector<float>> v, std::shared_ptr<DataVector<float>> n);
	~VisPrimitive();

	void Render(Camera* cam) override;
	int GenerateBuffers();
};