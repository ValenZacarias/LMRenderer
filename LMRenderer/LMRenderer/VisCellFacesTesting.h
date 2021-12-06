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
#include "PACalcCellCentroid.h"
#include "PACalcCellVolume.h"

template <typename TVertex, typename TIndex, typename TOwner, typename TNeighbour, typename TBounds>
class VisCellFacesTesting : public VisBase
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
	PACalcCellCentroid calcCellCentroid;
	PACalcCellVolume calcCellVolume;

	Shader shader;
public:

	VisCellFacesTesting(TVertex v, TIndex i, TOwner o, TNeighbour n, TBounds b, int cc)
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
	
	~VisCellFacesTesting()
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
		shader.setVec3("lightPos", LIGHT_POS.x, LIGHT_POS.y, LIGHT_POS.z);

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

		glDrawArrays(GL_TRIANGLES, 0, trisCount);

	}

	void GenerateBuffers()
	{

		// Initialize faceData with null faces
		auto faceData = make_shared<DataVector<Face>>(FACE);
		createFaces.Process(*ownerData, *faceData);

		// Generate cells
		auto cellData = make_shared<DataVector<Cell>>(CELL);
		generateCells.Process(*ownerData, *neighbourData, *boundaryData, *faceData, *cellData, cellCount);

		//Triangulate with Face data CREATION and Area Calc
		auto triVertexData = make_shared<DataVector<glm::vec3>>(POINT);
		DataVector<float> faceAreaData(FLOATVAL);
		triangulate.Process(*vertexData, *indexData, *triVertexData, *faceData, faceAreaData);

		// Calculate Cell centroids
		auto cellCentroidsData = make_shared<DataVector<glm::vec3>>(POINT);
		calcCellCentroid.Process(*faceData, *triVertexData, *cellData, *cellCentroidsData);

		// Calculate Cell Volumes
		DataVector<float> cellVolumeData(FLOATVAL);
		calcCellVolume.Process(*faceData, *triVertexData, *cellData, *cellCentroidsData, cellVolumeData);

		//Creating Cumulative Distribution function
		//vector<float> CDFData = calcCDF.Process(faceAreaData);
		vector<float> CDFData = calcCDF.Process(cellVolumeData);

		// Face index Sampling ---------------------------------------------------------
		// FaceIndex -> Sampled FaceIndex
		auto faceDataSample = make_shared<DataVector<Face>>(FACE);
		auto cellSample = make_shared<DataVector<Cell>>(CELL);

		// CELL RECONSTRUCTION TEST
		//reconstructCells.Process(*faceData, *faceDataSample, *cellData);
	
		// UNIFORM SAMPLING
		//uniformSample.Process(*faceData, *faceDataSample, 1.0); //UNIFORM
		//uniformSample.Process(*cellData, *cellSample, 0.05); //UNIFORM CELL SAMPLING

		//map<float, int> histo = uniformSample.Process_DebugHistogram(*faceData, *faceDataSample, 0.1, faceAreaData);
		
		// INVERSE TRANSFORM SAMPLING
		//invTransformSample.Process(CDFData, *faceData, *faceDataSample, 0.5); //INVERSE TRANSFORM
		invTransformSample.Process(CDFData, *cellData, *cellSample, 0.01); //INVERSE TRANSFORM CELL SAMPLING
		
		//map<float, int> histo = invTransformSample.Process_DebugHistogram(CDFData, *faceData, *faceDataSample, 0.1, faceAreaData);

		// SAMPLED CELL RECONSTRUCTION
		reconstructCells.Process(*faceData, *faceDataSample, *cellSample);

		// Sampled FaceIndex -> TriVertexData
		//DataVector<glm::vec3> triVertexSample = faceIndexTriangulate.Process(*triVertexData, *faceDataSample);
		DataVector<glm::vec3> triVertexSample = faceIndexTriangulate.Process(*triVertexData, *faceDataSample);
		
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

