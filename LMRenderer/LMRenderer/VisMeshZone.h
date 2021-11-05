#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "GLFWCanvas.h"

#include "VisBase.h"
#include "VisCell_bin.h"

#include "DataStructureBase.h"
#include "DataVector.h"
#include "Face.h"
#include "Cell.h"

#include "PATriangulate.h"
#include "PACalcNormals.h"

enum STATE { FAR, NEAR, VISIBLE };

class VisMeshZone : public VisBase
{
private:

	vector<shared_ptr<VisCell_bin<float>>>Sub_Visualizations;

	long zoneTrisCount;
	long renderedTrisCount;

	vector<glm::vec3> BoundingBox;
	GLuint VBO_BB;
	GLuint VAO_BB;
	GLuint EBO_BB;

	PACalcNormals calcnormals;

	Shader shader_bb;
	int lenght_bb;

public:

	STATE actualState;

	VisMeshZone()
	{
		//this->triVertexData = v;
		//this->cellCount = cc;

		//actualState = UNLOADED;

		//Shader diffuseShader("difusse_vertex_shader.txt", "difusse_fragment_shader.txt");
		//this->shader = diffuseShader;

		//Shader standardShader("light_vertex_shader.txt", "light_fragment_shader.txt");
		//this->shader_bb = standardShader;

		//triVertexData->StartRead();

		//BoundingBox = bb;
		//RenderBuffers_BB();

	}

	~VisMeshZone()
	{
		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
		//glDeleteBuffers(1, &EBO);
		cout << "VIS DELETED" << endl;
	}

	long GetTrisCount()
	{
		//return trisCount;
	}

	void Render(Camera* cam)
	{
		Render_BB(cam);

		switch (actualState)
		{
		case FAR:

			break;
		case NEAR:

			break;
		case VISIBLE:

		default:
			_impossible(true);
		}
	}


	void RenderBuffers()
	{

	}

	void Draw(Camera* cam)
	{
		/*if (actualState != RENDER) return;

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		shader.use();
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);

		glm::mat4 view;
		view = glm::lookAt(cam->cameraPos, cam->cameraPos + cam->cameraFront, cam->cameraUp);
		unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection;
		if (PERSPECTIVE_CAM)
			projection = glm::perspective(glm::radians(cam->FOV), 1280.0f / 720.0f, 0.01f, 5000.0f);
		else
			projection = glm::ortho(-2.0f * cam->FOV * 0.05f,
				+2.0f * cam->FOV * 0.05f,
				-1.15f * cam->FOV * 0.05f,
				+1.15f * cam->FOV * 0.05f,
				-100.0f, 100.0f);

		unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f * MESH_SCALE);
		unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, trisCount * 3);*/

	}

	void DeleteGPUBuffers()
	{
		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
		//GPUBuffersLoaded = false;
		//__nop();
		//cout << "GPU BUFFERS DELETED" << endl;
	}

	void DeleteMemoryBuffers()
	{
		//BufferData.clear();
	}

	// **************** BOUNDING BOX ************************

	void Render_BB(Camera* cam)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_BB);

		shader_bb.use();
		shader_bb.setVec3("lightColor", 0.3f, 0.6f, 0.1f);

		glm::mat4 view_bb;
		view_bb = glm::lookAt(cam->cameraPos, cam->cameraPos + cam->cameraFront, cam->cameraUp);
		unsigned int viewLoc_bb = glGetUniformLocation(shader_bb.ID, "view");
		glUniformMatrix4fv(viewLoc_bb, 1, GL_FALSE, glm::value_ptr(view_bb));

		glm::mat4 projection;
		if (PERSPECTIVE_CAM)
			projection = glm::perspective(glm::radians(cam->FOV), 1280.0f / 720.0f, 0.01f, 5000.0f);
		else
		{
			/*projection = glm::ortho(-2.0f * cam->FOV * 0.05f,
									+2.0f * cam->FOV * 0.05f,
									-1.15f * cam->FOV * 0.05f,
									+1.15f * cam->FOV * 0.05f,
									-100.0f, 100.0f);*/
			projection = glm::ortho(cam->ortho_frustrum[0],
									cam->ortho_frustrum[1],
									cam->ortho_frustrum[2],
									cam->ortho_frustrum[3],
									cam->ortho_frustrum[4],
									cam->ortho_frustrum[5]);
		}

		unsigned int projectionLoc_bb = glGetUniformLocation(shader_bb.ID, "projection");
		glUniformMatrix4fv(projectionLoc_bb, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model_bb = glm::mat4(1.0f * MESH_SCALE);
		unsigned int modelLoc_bb = glGetUniformLocation(shader_bb.ID, "model");
		glUniformMatrix4fv(modelLoc_bb, 1, GL_FALSE, glm::value_ptr(model_bb));

		glBindVertexArray(VAO_BB);

		glLineWidth(2.0f);
		glDrawElements(GL_LINES, lenght_bb, GL_UNSIGNED_INT, NULL);

	}

	int RenderBuffers_BB()
	{
		std::vector<int> indices{ 0, 1, 1, 5, 5, 4, 4, 0, // Front face
								  0, 3, 1, 2, 5, 6, 4, 7, // Middle edges
								  3, 2, 2, 6, 6, 7, 7, 3 };// Back face

		//Vertex array object init
		glGenVertexArrays(1, &VAO_BB);

		//Vertex buffer data initialize
		glGenBuffers(1, &VBO_BB);

		//Pass vertex data to buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO_BB);
		glBufferData(GL_ARRAY_BUFFER, BoundingBox.size() * sizeof(glm::vec3), &BoundingBox[0], GL_STATIC_DRAW);

		glBindVertexArray(VAO_BB);

		//Position Pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &EBO_BB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_BB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		lenght_bb = indices.size();

		return 0;
	}
};

