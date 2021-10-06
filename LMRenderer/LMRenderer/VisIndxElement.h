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

#include "DataStructureBase.h"
#include "DataVector.h"
#include "Face.h"
#include "Cell.h"

#include "PATriangulate.h"
#include "PACalcNormals.h"
#include "PAUniformSample.h"
#include "PAInvTransformSample.h"
#include "PAFaceIndexTriangulate.h"
#include "PACalcCDF.h"
#include "PAGenerateCells.h"
#include "PAReconstructCells.h"
#include "PACreateFaceData.h"

template <typename TVertex, typename TIndex, typename TOwner, typename TNeighbour, typename TBounds>
class VisIndxElement : public VisBase
{
private:

	TVertex vertexData;
	TIndex indexData;
	TOwner ownerData;
	TNeighbour neighbourData;
	TBounds boundaryData;
	int cellCount;

	GLuint VBO;
	GLuint VAO;
	GLuint EBO;
	long trisCount;

	PATriangulate triangulate;
	PACalcNormals calcnormals;
	PAUniformSample uniformSample;
	PAInvTransformSample invTransformSample;
	PAFaceIndexTriangulate faceIndexTriangulate;
	PACalcCDF calcCDF;
	PAGenerateCells generateCells;
	PAReconstructCells reconstructCells;
	PACreateFaceData createFaces;

	Shader shader;
public:

	VisIndxElement(TVertex v, TIndex i, TOwner o, TNeighbour n, TBounds b, int cc)
	{
		this->vertexData = v;
		this->indexData = i;
		this->ownerData = o;
		this->neighbourData = n;
		this->boundaryData = b;
		this->cellCount = cc;

		Shader diffuseShader("difusse_vertex_shader.txt", "difusse_fragment_shader.txt");
		this->shader = diffuseShader;

		GenerateBuffers();
	}
	
	~VisIndxElement()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		cout << "VIS DELETED" << endl;
	}

	void Render(Camera* cam)
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

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(cam->FOV), 1280.0f / 720.0f, 0.01f, 500.0f);
		unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f);
		unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		//FIX: hay que variabilizar la cantidad de triangulos segun la malla
		glDrawArrays(GL_TRIANGLES, 0, trisCount);

	}

	void GenerateBuffers()
	{
		DataVector<glm::vec3> _triVertexData(POINT);
		auto triVertexData = make_shared<DataVector<glm::vec3>>(_triVertexData);

		DataVector<Face> _faceIndexData(FACE);
		auto faceIndexData = make_shared<DataVector<Face>>(_faceIndexData);

		createFaces.Process(*ownerData, *faceIndexData);

		DataVector<Cell> _cellData(CELL);
		auto cellData = make_shared<DataVector<Cell>>(_cellData);

		generateCells.Process(*ownerData, *neighbourData, *boundaryData, *faceIndexData, *cellData, cellCount);

		//Triangulate with Face data CREATION and Area Calc
		DataVector<float> faceAreaData(FLOATVAL);
		triangulate.Process(*vertexData, *indexData, *triVertexData, *faceIndexData, faceAreaData);

		//Creating Cumulative Distribution function
		vector<float> CDFData = calcCDF.Process(faceAreaData);

		// Face index Sampling ---------------------------------------------------------
		// FaceIndex -> Sampled FaceIndex
		DataVector<Face> _faceIndexSample(FACE);
		auto faceIndexSample = make_shared<DataVector<Face>>(_faceIndexSample);

		DataVector<Cell> _cellSample(CELL);
		auto cellSample = make_shared<DataVector<Cell>>(_cellSample);

		// CELL RECONSTRUCTION TEST
		//reconstructCells.Process(*faceIndexData, *faceIndexSample, *cellData);
	
		// UNIFORM SAMPLING
		//uniformSample.Process(*faceIndexData, *faceIndexSample, 1.0); //UNIFORM
		uniformSample.Process(*cellData, *cellSample, 0.05); //UNIFORM CELL SAMPLING
		//map<float, int> histo = uniformSample.Process_DebugHistogram(*faceIndexData, *faceIndexSample, 0.1, faceAreaData);
		
		// INVERSE TRANSFORM SAMPLING
		//invTransformSample.Process(CDFData, *faceIndexData, *faceIndexSample, 0.5); //INVERSE TRANSFORM
		//map<float, int> histo = invTransformSample.Process_DebugHistogram(CDFData, *faceIndexData, *faceIndexSample, 0.1, faceAreaData);

		// SAMPLED CELL RECONSTRUCTION
		reconstructCells.Process(*faceIndexData, *faceIndexSample, *cellSample);

		// Sampled FaceIndex -> TriVertexData
		//DataVector<glm::vec3> triVertexSample = faceIndexTriangulate.Process(*triVertexData, *faceIndexSample);
		DataVector<glm::vec3> triVertexSample = faceIndexTriangulate.Process(*triVertexData, *faceIndexSample);
		
		// Non face index sampling ---------------------------------------------------------
		//DataVector<glm::vec3> triVertexSample = uniformSample.Process(*triVertexData, 0.1f);

		// NON SAMPLED DATA
		//DataVector<glm::vec3> triVertexSample = *triVertexData;

		trisCount = triVertexSample.GetSize();
		DataVector<glm::vec3> normaldata = calcnormals.ProcessVec3(triVertexSample);

		int n = triVertexSample.GetSize();
		std::vector<glm::vec3> bufferdata{};

		for (int i = 0; i < n; i++)
		{
			bufferdata.push_back(triVertexSample.GetData(i));
			bufferdata.push_back(normaldata.GetData(i));
		}

		//Vertex array object init
		glGenVertexArrays(1, &VAO);

		//Vertex buffer data initialize
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		//Pass vertex data to buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, bufferdata.size() * sizeof(glm::vec3), &bufferdata[0], GL_STATIC_DRAW);

		glBindVertexArray(VAO);

		//Position Pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		////Normal Pointer
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}


};

