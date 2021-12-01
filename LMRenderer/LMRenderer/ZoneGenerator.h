#pragma once
#include "DataStructureBase.h"
#include "DataVector.h"
#include "DataBinFile.h"
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
#include "PACalcBoundingBox.h"

#include "VisBase.h"
#include "VisGroup.h"
#include "VisGrid.h"
#include "VisPrimitive.h"
#include "VisCellFacesTesting.h"
#include "VisCell_bin.h"
#include "VisMeshZone.h"

#include "PMeshParser.h"

using namespace std;
class ZoneGenerator
{
private:
	PMeshParser Parser;
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
	PACalcBoundingBox calcBoundingBox;

	int generatedZones = 0;

public:

	ZoneGenerator() 
	{
		Parser = PMeshParser();
	};
	shared_ptr<VisMeshZone> GenerateZone(string meshFilePath);
	vector<shared_ptr<VisMeshZone>> GenerateRepeatedZones(string meshFilePath, int zoneCount, int columns, int rows);
	void loadSubVisData(shared_ptr<DataBinFile<glm::vec3>> fileDataBuffer, DataVector<glm::vec3>& triVertexData, DataVector<Face>& faceDataBuffer);
	void loadSubVisDataOffset(shared_ptr<DataBinFile<glm::vec3>> fileDataBuffer, DataVector<glm::vec3>& triVertexData, DataVector<Face>& faceDataBuffer, glm::vec3 offset);
};

