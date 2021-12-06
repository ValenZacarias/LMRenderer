#pragma once
#include <chrono>
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

#include "DataStructureBase.h"
#include "DataBinFile.h"
#include "DataVector.h"
#include "Face.h"
#include "Cell.h"

#include "PATriangulate.h"
#include "PACalcNormals.h"

using namespace chrono;

enum STATE { UNLOADED, LOADED };

class VisCell_bin : public VisBase
{
private:

	shared_ptr<DataBinFile<glm::vec3>> triVertexData;
	std::vector<glm::vec3> BufferData{};

	GLuint VBO;
	GLuint VAO;
	long trisCount = 0;
	bool GPUBuffersLoaded = false;
	bool MemoryBuffersLoaded = false;

	PATriangulate triangulate;
	PACalcNormals calcnormals;

	Shader shader;

public:

	STATE actualState = UNLOADED;

	VisCell_bin(shared_ptr<DataBinFile<glm::vec3>> v)
	{
		this->triVertexData = v;
		//this->cellCount = cc;
		this->trisCount = v->GetSize() / 3;
		
		actualState = UNLOADED;

		Shader diffuseShader("difusse_vertex_shader.txt", "difusse_fragment_shader.txt");
		this->shader = diffuseShader;
	}

	~VisCell_bin()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		cout << "VIS DELETED" << endl;
	}

	long GetTrisCount()
	{
		return trisCount;
	}

	void Update(Camera* cam)
	{
		switch (actualState)
		{
			case UNLOADED:

				// Unload data from memory
				if(MemoryBuffersLoaded)
					DeleteMemoryBuffers();

				break;
			case LOADED:
				// Load data to memory, then to GPU
				if (!MemoryBuffersLoaded)
				{
					LoadFileData();
					GenerateBuffers();
				}
				else
					Draw(cam); // Drawcall
				break;
			default:
				_impossible(true);
		}
	}

	void LoadFileData()
	{
		_expects(triVertexData->GetSize() > 0);

		steady_clock::time_point begin = steady_clock::now();

		triVertexData->StartRead();

		trisCount = triVertexData->GetSize()/3;
		//DataVector<glm::vec3> normaldata = calcnormals.ProcessVec3(*triVertexData);

		int n = triVertexData->GetSize();

		// Normals from file
		//for (int i = 0; i < n; i++){ BufferData.push_back(triVertexData->GetData(i)); }

		// File full data
		BufferData.resize(n);
		triVertexData->GetFullData(BufferData);

		//for (int i = 0; i < n; i++)
		//{
		//	BufferData.push_back(triVertexData->GetData(i));
		//	BufferData.push_back(normaldata.GetData(i));
		//}

		triVertexData->CloseFile();

		steady_clock::time_point end = steady_clock::now();
		//cout << " Data read and Normals Processing = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
		cout << " Data read = " << duration_cast<milliseconds>(end - begin).count() << "[ms]" << endl;
		cout << " Loaded " << triVertexData->GetSize() << " vectors" << endl;

		//__nop();

		if (BufferData.size() == 0)
		{
			//cout << "VIS SAMPLE CANT LOAD DATA FROM FILE " << endl;
		}

		actualState = LOADED;

		//cout << "VIS SAMPLE LOADED " << trisCount << " TRIANGLES" << endl;

		//GenerateBuffers();

		MemoryBuffersLoaded = true;
	}

	void GenerateBuffers()
	{
		_expects(BufferData.size() > 0);

		//Vertex array object init
		glGenVertexArrays(1, &VAO);

		//Vertex buffer data initialize
		glGenBuffers(1, &VBO);

		//Pass vertex data to buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, BufferData.size() * sizeof(glm::vec3), &BufferData[0], GL_STATIC_DRAW);

		glBindVertexArray(VAO);

		//Position Pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		////Normal Pointer
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		GPUBuffersLoaded = true;

		//__nop();
	}

	void Draw(Camera* cam)
	{
		if (actualState != LOADED) return;

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		shader.use();
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", LIGHT_POS.x, LIGHT_POS.y, LIGHT_POS.z);

		unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view_matrix));
		
		glm::mat4 projection;
		if (PERSPECTIVE_CAM)
			projection = cam->perspective_matrix;
		else
			projection = cam->ortho_matrix;

		unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f * MESH_SCALE);
		unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, trisCount*3);

	}

	//void DeleteGPUBuffers()
	//{
	//	glDeleteVertexArrays(1, &VAO);
	//	glDeleteBuffers(1, &VBO);
	//	GPUBuffersLoaded = false;
	//	//__nop();
	//	cout << "GPU BUFFERS DELETED" << endl;
	//}

	void DeleteMemoryBuffers()
	{
		cout << "VIS SAMPLE UNLOADED " << trisCount << " TRIANGLES" << endl;
		BufferData.clear();
		glDeleteVertexArrays(1, &VAO);
		glDeleteRenderbuffers(1, &VBO);
		glDeleteBuffers(1, &VBO);

		MemoryBuffersLoaded = false;
	}

};

