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

glm::vec3 cubePositions[] = {
	glm::vec3(-1.7f,  2.0f, 0.0f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


VisualizationPrimitive::VisualizationPrimitive(GLuint* vao, GLuint* vbo)
{
	//this->VAO = vao;
	//this->VBO = vbo;
	//Shader diffuseShader("difusse_vertex_shader.txt", "difusse_fragment_shader.txt");
	//this->shader = diffuseShader;

};

VisualizationPrimitive::VisualizationPrimitive(std::shared_ptr<DataVectorTest<float>> v, 
											   std::shared_ptr<DataVectorTest<float>> n)
{
	this->vertexdata = v;
	this->normaldata = n;
	Shader diffuseShader("difusse_vertex_shader.txt", "difusse_fragment_shader.txt");
	this->shader = diffuseShader;

	GenerateBuffers();

};

void VisualizationPrimitive::Render(Camera* cam)
{
	for (unsigned int i = 0; i < 5; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		shader.use();
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);

		glm::mat4 view;
		view = glm::lookAt(cam->cameraPos, cam->cameraPos + cam->cameraFront, cam->cameraUp);
		unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//projection: View -> Clip
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(cam->FOV), 1280.0f / 720.0f, 1.0f, 1000.0f);
		unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

int VisualizationPrimitive::GenerateBuffers()
{
	int n = vertexdata->GetSize();
	std::vector<float> bufferdata {};

	for (int i = 0; i < n; i+=3)
	{
		bufferdata.push_back(vertexdata->GetData(i));
		bufferdata.push_back(vertexdata->GetData(i + 1));
		bufferdata.push_back(vertexdata->GetData(i + 2));

		bufferdata.push_back(normaldata->GetData(i));
		bufferdata.push_back(normaldata->GetData(i + 1));
		bufferdata.push_back(normaldata->GetData(i + 2));

		//cout << "vertice " << i / 6 << endl;
		//std::cout << bufferdata[i] << ", " << bufferdata[i+1] << ", " << bufferdata[i+2] << ", " << endl;
		//std::cout << bufferdata[i+3] << ", " << bufferdata[i+4] << ", " << bufferdata[i+5]  << endl;
		//cout << " " << endl;
	}

	//Vertex array object init
	glGenVertexArrays(1, &VAO);

	//Vertex buffer data initialize
	glGenBuffers(1, &VBO);

	//Pass vertex data to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferdata.size() * sizeof(float), bufferdata.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	//Position Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	////Normal Pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return 0;
}