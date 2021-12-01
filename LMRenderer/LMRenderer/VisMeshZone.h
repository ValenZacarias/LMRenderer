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
	int ZONE_ID;
	int loadedLevel = -1;

	vector<shared_ptr<VisCell_bin>> subVisualizations{};

	long loadedTrisCount = 0;

	vector<glm::vec3> BoundingBox;
	glm::vec3 Centroid;
	GLuint VBO_BB;
	GLuint VAO_BB;
	GLuint EBO_BB;

	Shader shader_bb;
	int vertexCount_bb;

	float frus_dist_left = 0.0f;
	float frus_dist_right = 0.0f;
	float frus_dist_bottom = 0.0f;
	float frus_dist_top = 0.0f;

public:

	ZONE_STATE actualState = FAR;

	VisMeshZone()
	{
		actualState = FAR;

		Shader standardShader("light_vertex_shader.txt", "light_fragment_shader.txt");
		this->shader_bb = standardShader;
		ZONE_ID = VIS_ID_COUNTER;
		VIS_ID_COUNTER++;
	}

	VisMeshZone(vector<glm::vec3> bb)
	{
		actualState = FAR;

		// Bounding Box Init
		Shader standardShader("light_vertex_shader.txt", "light_fragment_shader.txt");
		this->shader_bb = standardShader;
		BoundingBox = bb;
		CalcCentroid_BB();
		GenerateBuffers_BB();

	}

	~VisMeshZone()
	{
		cout << "VIS ZONE DELETED" << endl;
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
		//subVisTrisCount.push_back(vis->GetTrisCount());
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

	int GetPossibleLevel(int triCountLimit)
	{
		// tenemos que acumular los triangulos de cada nivel y compararlos con el limite
		// iniciamos con el nivel cero, si este ya no cumple con < limit, devolvemos -1 sino seguimos acumulando 
		// hasta encontrar el nivel que haga que lo acumulado pase el limite, ahi devolvemos como nivel limite de cargado
		// el anterior que cumplio la condicion

		int candidateVisIndex = -1;
		//int accumulatedTriCount = subVisualizations[0]->GetTrisCount();
		int accumulatedTriCount = 0;

		for (int i = 0; i < subVisualizations.size(); i++)
		{
			if (accumulatedTriCount + subVisualizations[i]->GetTrisCount() <= triCountLimit)
			{
				accumulatedTriCount += subVisualizations[i]->GetTrisCount();
				candidateVisIndex++;
			}
		}

		if (candidateVisIndex == loadedLevel)
			return -1;

		return candidateVisIndex;
	}

	int GetTrisLoaded() override
	{
		return loadedTrisCount;
	}

	//int LoadLevel(int index) devuelve la cantidad de triangulos que va a cargar
	void LoadLevel(int highestPossibleLevel)
	{
		if(highestPossibleLevel == -1) _impossible(true);

		for (int i = 0; i < highestPossibleLevel + 1; i++)
		{
			//subVisualizations[i]->LoadFileData();
			//subVisualizations[i]->LoadFileData();
			//subVisualizations[i]->GenerateBuffers();
			if (subVisualizations[i]->actualState != LOADED)
			{
				subVisualizations[i]->actualState = LOADED;
				loadedTrisCount += subVisualizations[i]->GetTrisCount();
			}

		}

		loadedLevel = highestPossibleLevel;

		// EN PRINCIPIO NO DEBERIA SER NECESARIO SACAR EL MAS BAJO
		// SI EN EL PEOR DE LOS CASOS EL NIVEL N-1 ESTA TOTALMENTE CONTENIDO EN EL SIGUIENTE
		// 
		//if (loadedLevel != -1) subVisualizations[loadedLevel]->actualState = UNLOADED;
		//subVisualizations[highestLevel]->actualState = LOADED; //este es el estado objetivo

		//loadedLevel = index;
	}

	int UnloadLastLevel()
	{
		//if (loadedLevel == -1 || loadedLevel > 2)
		//{
		//	// caso donde la visualizacion es descargable pero no tiene nada cargado
		//	// de alguna forma llegan visualizaciones con loaded level >2 , parece ser que se llenan de basura
		//	// se reinicializan todos los datos en ese caso

		//	loadedLevel = -1;
		//	loadedTrisCount = 0;

		//	return 0;
		//}

		if (loadedLevel == -1)
			return 0;

		int trisLoadedBeforeUnload = subVisualizations[loadedLevel]->GetTrisCount();
		if (loadedTrisCount - trisLoadedBeforeUnload > 0)
			loadedTrisCount -= trisLoadedBeforeUnload;
		else
			loadedTrisCount = 0;
		//subVisualizations[loadedLevel]->DeleteMemoryBuffers();
		subVisualizations[loadedLevel]->actualState = UNLOADED;
		loadedLevel--;

		return trisLoadedBeforeUnload;
	}

	void Render(Camera* cam)
	{

		Render_BB(cam);

		if (isInsidefrustum(cam))
			actualState = VISIBLE;
		else
			actualState = NEAR;

		for (int i = 0; i < subVisualizations.size(); i++)
		{
			subVisualizations[i]->Update(cam);
		}
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

