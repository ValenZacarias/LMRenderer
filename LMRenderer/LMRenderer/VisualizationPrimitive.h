#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"
//#include "GLFWCanvas.h"

class VisualizationPrimitive : public VisualizationBase
{ 
private:
	GLuint* VAO; //Deberia ser puntero al vao? Mepa que no es necesario dado que solo es el ID y no el array en si
	GLuint* VBO;
	Shader shader;
public:
	VisualizationPrimitive(GLuint* vao, GLuint* vbo);
	//void Render(Camera* cam, Shader* shader) override;
	void Render(Camera* cam) override;
};