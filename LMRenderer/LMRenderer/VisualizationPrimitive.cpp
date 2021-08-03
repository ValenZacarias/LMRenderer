#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"
#include "VisualizationPrimitive.h"
#include "GLFWCanvas.h"

//glm::vec3 LP = glm::vec3(0.0f, 2.0f, 0.0f);

glm::vec3 cubePositions[] = {
	//glm::vec3(0.0f,  0.0f,  0.0f),
	//glm::vec3(2.0f,  5.0f, 0.0f),
	//glm::vec3(-1.5f, -2.2f, -2.5f),
	//glm::vec3(-3.8f, -2.0f, -12.3f),
	//glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


VisualizationPrimitive::VisualizationPrimitive(GLuint* vao)
{
	this->VAO = vao;
};

void VisualizationPrimitive::Render(Camera* cam, Shader* shader)
{
	//std::cout << "VPRIM RENDER" << std::endl;


	//std::cout << "PrimShader ID: " << shader->ID << std::endl;
	for (unsigned int i = 0; i < 5; i++)
	{
		// Por que tengo que setear todos los datos del shader aca, si yo quiero por ejemplo 
		// que la posicion de la luz sea fija para todos los VizPrimitives, y VizGroups
		shader->use();
		shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader->setVec3("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);

		glm::mat4 view;
		view = glm::lookAt(cam->cameraPos, cam->cameraPos + cam->cameraFront, cam->cameraUp);
		unsigned int viewLoc = glGetUniformLocation(shader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//projection: View -> Clip
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(cam->FOV), 1280.0f / 720.0f, 1.0f, 1000.0f);
		unsigned int projectionLoc = glGetUniformLocation(shader->ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(*VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	////Render cubes --------------------------------------------------------------------------------------
	//	//Todo esto va a visualization, la ventana tendra un puntero a vis
	//	for (unsigned int i = 0; i < 1; i++)
	//	{
	//		shader->use();

	//		shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	//		
	//		//Todo esto va a la funcion de render de Canvas ----------------------------------
	//		//diffuseShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//		//diffuseShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

	//		//unsigned int viewLoc = glGetUniformLocation(diffuseShader.ID, "view");
	//		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//		//unsigned int projectionLoc = glGetUniformLocation(diffuseShader.ID, "projection");
	//		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	//		//--------------------------------------------------------------------------------

	//		glm::mat4 model = glm::mat4(1.0f);
	//		//model = glm::translate(model, cubePositions[i]);
	//		//model = glm::rotate(model, (float)glfwGetTime() * 1.5f, cubePositions[i]);
	//		//unsigned int modelLoc = glGetUniformLocation(diffuseShader.ID, "model");
	//		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//		glBindVertexArray(VAO);
	//		std::cout << "VAO: " << VAO << std::endl;
	//		glDrawArrays(GL_TRIANGLES, 0, 36);
	//	}
}