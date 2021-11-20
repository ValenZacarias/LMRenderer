#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "GLFWCanvas.h"
#include "DrawLine.h"
#include "DrawPoint.h"

#include "VisBase.h"
#include "VisCell_bin.h"

#include "DataStructureBase.h"
#include "DataVector.h"
#include "Face.h"
#include "Cell.h"

#include "PATriangulate.h"
#include "PACalcNormals.h"

enum ZONE_STATE { FAR, NEAR, VISIBLE };

class VisMeshZone : public VisBase
{
private:

	//vector<shared_ptr<VisCell_bin<float>>>Sub_Visualizations;
	int loadedLevel = -1;

	//vector<shared_ptr<VisBase>> subVisualizations{};
	vector<shared_ptr<VisCell_bin>> subVisualizations{};
	vector<int> subVisTrisCount{};

	long zoneTrisCount;
	long renderedTrisCount;

	vector<glm::vec3> BoundingBox;
	glm::vec3 Centroid;
	GLuint VBO_BB;
	GLuint VAO_BB;
	GLuint EBO_BB;

	PACalcNormals calcnormals;

	Shader shader_bb;
	int vertexCount_bb;

	DrawPoint p4;

	float frus_dist_left = 0.0f;
	float frus_dist_right = 0.0f;
	float frus_dist_bottom = 0.0f;
	float frus_dist_top = 0.0f;

public:

	ZONE_STATE actualState;

	VisMeshZone()
	{
		Shader standardShader("light_vertex_shader.txt", "light_fragment_shader.txt");
		this->shader_bb = standardShader;
	}

	//VisMeshZone( vector<VisBase*> subVis ,vector<glm::vec3> bb)
	VisMeshZone(vector<glm::vec3> bb)
	{
		actualState = FAR;

		//for(int i = 0; subVis.size)

		// Bounding Box Init
		Shader standardShader("light_vertex_shader.txt", "light_fragment_shader.txt");
		this->shader_bb = standardShader;
		BoundingBox = bb;
		CalcCentroid_BB();
		GenerateBuffers_BB();

	}

	~VisMeshZone()
	{
		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
		//glDeleteBuffers(1, &EBO);
		cout << "VIS DELETED" << endl;
	}

	void SetBoundingBox(vector<glm::vec3> bb)
	{
		BoundingBox = bb;
		CalcCentroid_BB();
		GenerateBuffers_BB();
	}

	void AddSubVisualization(shared_ptr<VisCell_bin> vis)
	{
		subVisualizations.push_back(vis);
		subVisTrisCount.push_back(vis->GetTrisCount());
	}

	bool isInsidefrustum(Camera* cam)
	{
		if (cam->pIsInsideFrustrum(Centroid))
			return true;

		for (int i = 0; i < BoundingBox.size(); i++)
		{
			if ( cam->pIsInsideFrustrum(BoundingBox[i]))
			{
				return true;
			}
		}

		return false;
	}

	int GetCurrentLevel() override
	{
		return loadedLevel;
	}

	bool IsVisible() override
	{

		if (actualState == VISIBLE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int GetPossibleLevel(int triCount)
	{
		int candidateVisIndex = -1;

		for (int i = 0; i < subVisTrisCount.size(); i++)
		{
			if (subVisTrisCount[i] <= triCount)
				candidateVisIndex = i;
		}

		if (candidateVisIndex == loadedLevel)
			return -1;

		return candidateVisIndex;
	}

	//int LoadLevel(int index) devuelve la cantidad de triangulos que va a cargar
	void LoadLevel(int index)
	{
		if(index == -1) _impossible(true);

		// EN PRINCIPIO NO DEBERIA SER NECESARIO SACAR EL MAS BAJO
		// SI EN EL PEOR DE LOS CASOS EL NIVEL N-1 ESTA TOTALMENTE CONTENIDO EN EL SIGUIENTE
		// 
		//if (loadedLevel != -1) subVisualizations[loadedLevel]->actualState = UNLOADED;
		subVisualizations[index]->actualState = LOADED; //este es el estado objetivo

		loadedLevel = index;
	}

	void Render(Camera* cam)
	{
		//p4.Render(cam);
		//p5.Render(cam);
		//p6.Render(cam);
		//p7.Render(cam);
		Render_BB(cam);

		if (isInsidefrustum(cam))
			actualState = VISIBLE;
		else
			actualState = NEAR;

		for (int i = 0; i < subVisualizations.size(); i++)
		{
			subVisualizations[i]->Update(cam);
		}

		//switch (actualState)
		//{
		//case FAR:

		//	break;
		//case NEAR:

		//	break;
		//case VISIBLE:

		//default:
		//	_impossible(true);
		//}
	}


	void RenderBuffers()
	{

	}

	void Draw(Camera* cam)
	{

	}

	void DeleteGPUBuffers()
	{

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

		if(actualState != VISIBLE)
			shader_bb.setVec3("lightColor", BB_COLOR_HIDDEN.r, BB_COLOR_HIDDEN.g, BB_COLOR_HIDDEN.b);
		else
			shader_bb.setVec3("lightColor", BB_COLOR_VISIBLE.r, BB_COLOR_VISIBLE.g, BB_COLOR_VISIBLE.b);

		unsigned int viewLoc_bb = glGetUniformLocation(shader_bb.ID, "view");
		glUniformMatrix4fv(viewLoc_bb, 1, GL_FALSE, glm::value_ptr(cam->view_matrix));

		glm::mat4 projection;
		if (PERSPECTIVE_CAM)
			projection = cam->perspective_matrix;
		else
			projection = cam->ortho_matrix;

		unsigned int projectionLoc_bb = glGetUniformLocation(shader_bb.ID, "projection");
		glUniformMatrix4fv(projectionLoc_bb, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model_bb = glm::mat4(1.0f * MESH_SCALE);
		unsigned int modelLoc_bb = glGetUniformLocation(shader_bb.ID, "model");
		glUniformMatrix4fv(modelLoc_bb, 1, GL_FALSE, glm::value_ptr(model_bb));

		glBindVertexArray(VAO_BB);

		glLineWidth(2.0f);
		glDrawElements(GL_LINES, vertexCount_bb, GL_UNSIGNED_INT, NULL);

	}

	int GenerateBuffers_BB()
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

		vertexCount_bb = indices.size();

		return 0;
	}

	void CalcCentroid_BB()
	{
		for (int i = 0; i < BoundingBox.size(); i++)
		{
			Centroid = Centroid + BoundingBox[i];
		}
		Centroid /= 8;
	}
};

