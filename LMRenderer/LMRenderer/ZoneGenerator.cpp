#include <iostream>
#include <fstream>
#include <stdio.h>

#include "ZoneGenerator.h"
#include "Globals.h"
#include "PMeshParser.h"

using namespace std;

shared_ptr<VisMeshZone> ZoneGenerator::GenerateZone(string meshFilePath)
{
	shared_ptr<VisMeshZone> visZone = make_shared<VisMeshZone>();
	int subVisPerZone = 3;
	bool forceFileCreation = false;

	auto DataIndex = Parser.ParseFaces(meshFilePath + "_faces.txt");
	auto DataVertex = Parser.ParsePoints(meshFilePath + "_points.txt");
	auto DataOwner = Parser.ParseCells(meshFilePath + "_owner.txt");
	auto DataNeighbour = Parser.ParseCells(meshFilePath + "_neighbour.txt");
	int cellCount = Parser.ParseCellCount(meshFilePath + "_owner.txt");
	auto DataBoundary = Parser.ParseBoundary(meshFilePath + "_boundary.txt");

	auto faceData = make_shared<DataVector<Face>>(FACE);
	createFaces.Process(*DataOwner, *faceData);

	// Generate cells
	auto cellData = make_shared<DataVector<Cell>>(CELL);
	generateCells.Process(*DataOwner, *DataNeighbour, *DataBoundary, *faceData, *cellData, cellCount);

	// Triangulate with Face data CREATION and Area Calc
	auto triVertexData = make_shared<DataVector<glm::vec3>>(POINT);
	DataVector<float> faceAreaData(FLOATVAL);
	triangulate.Process(*DataVertex, *DataIndex, *triVertexData, *faceData, faceAreaData);

	// Calc Bounding Box
	vector<glm::vec3> BBVertices{};
	calcBoundingBox.Process(*DataVertex, BBVertices);
	visZone->SetBoundingBox(BBVertices);

	// SUB VIS GENERATION --------------------------------------------------------------------------------------------------

	// First sample
	auto faceDataBuffer = make_shared<DataVector<Face>>(FACE);
	auto cellDataSample = make_shared<DataVector<Cell>>(CELL);

	int SampleSize = 1000;
	int sampleGrowFactor = 2;
	int sampleNumber = 0;
	float samplePercent = 0;

	int totalPrimitiveSize = cellData->GetSize();

	//for (int i = 0; i < subVisPerZone; i++)
	while (SampleSize < totalPrimitiveSize)
	{
		samplePercent = (float)SampleSize / totalPrimitiveSize;
		uniformSample.Process(cellData, cellDataSample, samplePercent);
		reconstructCells.Process(*faceData, *faceDataBuffer, *cellDataSample);

		string filename = "bin/zone_" + to_string(generatedZones) + "_" + to_string(sampleNumber);

		if (forceFileCreation)
		{
			auto fileDataBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);
			loadSubVisData(fileDataBuffer, *triVertexData, *faceDataBuffer);

			auto vis = make_shared<VisCell_bin>(fileDataBuffer);
			visZone->AddSubVisualization(vis);
		}
		else
		{
			auto fileDataBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);

			if (!fileDataBuffer->FileExists())
			{
				loadSubVisData(fileDataBuffer, *triVertexData, *faceDataBuffer);
			}

			auto vis = make_shared<VisCell_bin>(fileDataBuffer);
			visZone->AddSubVisualization(vis);
		}

		sampleNumber++;
		SampleSize = SampleSize * sampleGrowFactor ^ sampleNumber;

		cellDataSample->Clear();
		faceDataBuffer->Clear();
	}

	// LAST VIS: FULL MESH ZONE (NO SAMPLING)
	string filename = "bin/zone_" + to_string(generatedZones) + "_" + to_string(sampleNumber);
	auto lastFileDataBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);
	if (!lastFileDataBuffer->FileExists())
	{
		reconstructCells.Process(*faceData, *faceDataBuffer, *cellData);
		loadSubVisData(lastFileDataBuffer, *triVertexData, *faceDataBuffer);
	}
	
	auto vis = make_shared<VisCell_bin>(lastFileDataBuffer);
	visZone->AddSubVisualization(vis);

	return visZone;
}

vector<shared_ptr<VisMeshZone>> ZoneGenerator::GenerateRepeatedZones(string meshFilePath, int zoneCount, int columns, int rows)
{
	vector<shared_ptr<VisMeshZone>> zones {};

	int subVisPerZone = 3;
	bool forceFileCreation = false;

	auto DataIndex = Parser.ParseFaces(meshFilePath + "_faces.txt");
	auto DataVertex = Parser.ParsePoints(meshFilePath + "_points.txt");
	auto DataOwner = Parser.ParseCells(meshFilePath + "_owner.txt");
	auto DataNeighbour = Parser.ParseCells(meshFilePath + "_neighbour.txt");
	int cellCount = Parser.ParseCellCount(meshFilePath + "_owner.txt");
	auto DataBoundary = Parser.ParseBoundary(meshFilePath + "_boundary.txt");

	auto faceData = make_shared<DataVector<Face>>(FACE);
	createFaces.Process(*DataOwner, *faceData);

	// Generate cells
	auto cellData = make_shared<DataVector<Cell>>(CELL);
	generateCells.Process(*DataOwner, *DataNeighbour, *DataBoundary, *faceData, *cellData, cellCount);

	// Triangulate with Face data CREATION and Area Calc
	auto triVertexData = make_shared<DataVector<glm::vec3>>(POINT);
	DataVector<float> faceAreaData(FLOATVAL);
	triangulate.Process(*DataVertex, *DataIndex, *triVertexData, *faceData, faceAreaData);
	
	// Calc Bounding Box
	vector<glm::vec3> BBVertices{};
	calcBoundingBox.Process(*DataVertex, BBVertices);

	glm::vec3 x_offset = BBVertices[1] - BBVertices[0];
	glm::vec3 z_offset = BBVertices[4] - BBVertices[0];
	
	// First sample
	auto faceDataBuffer = make_shared<DataVector<Face>>(FACE);
	auto cellDataSample = make_shared<DataVector<Cell>>(CELL);

	for (int j = 0; j < zoneCount; j++) // Zone loop
	{
		shared_ptr<VisMeshZone> visZone = make_shared<VisMeshZone>();

		vector<glm::vec3> BBOffseted;
		glm::vec3 offset = (x_offset * (float)(j % columns)) + z_offset * (float)(trunc(j/ rows));
		for (int p = 0; p < BBVertices.size(); p++) { BBOffseted.push_back(BBVertices[p] + offset); }
		visZone->SetBoundingBox(BBOffseted);

		int SampleSize = 100;
		int sampleGrowFactor = 2;
		int sampleNumber = 0;
		float samplePercent = 0;
		int totalPrimitiveSize = cellData->GetSize();

		//for (int i = 0; i < subVisPerZone; i++) // Samplings Loop
		while (SampleSize < totalPrimitiveSize) // Samplings Loop
		{
			samplePercent = (float)SampleSize / totalPrimitiveSize;
			uniformSample.Process(cellData, cellDataSample, samplePercent);
			reconstructCells.Process(*faceData, *faceDataBuffer, *cellDataSample);

			string filename = "bin/zone_" + to_string(j) + "_" + to_string(sampleNumber);

			if (forceFileCreation)
			{
				auto fileDataBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);

				loadSubVisDataOffset(fileDataBuffer, *triVertexData, *faceDataBuffer, offset);

				auto vis = make_shared<VisCell_bin>(fileDataBuffer);
				visZone->AddSubVisualization(vis);
			}
			else
			{
				auto fileDataBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);

				if (!fileDataBuffer->FileExists())
				{
					loadSubVisDataOffset(fileDataBuffer, *triVertexData, *faceDataBuffer, offset);
				}

				auto vis = make_shared<VisCell_bin>(fileDataBuffer);
				visZone->AddSubVisualization(vis);
			}

			sampleNumber++;
			SampleSize = SampleSize * sampleGrowFactor ^ sampleNumber;

			cellDataSample->Clear();
			faceDataBuffer->Clear();
		}

		// LAST VIS: FULL MESH ZONE (NO SAMPLING)
		string filename = "bin/zone_" + to_string(j) + "_" + to_string(sampleNumber);
		auto lastFileDataBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);
		if (!lastFileDataBuffer->FileExists())
		{
			reconstructCells.Process(*faceData, *faceDataBuffer, *cellData);
			loadSubVisDataOffset(lastFileDataBuffer, *triVertexData, *faceDataBuffer, offset);
		}

		auto vis = make_shared<VisCell_bin>(lastFileDataBuffer);
		visZone->AddSubVisualization(vis);

		zones.push_back(visZone);

		cellDataSample->Clear();
		faceDataBuffer->Clear();
	}
	return zones;
}

void ZoneGenerator::loadSubVisData(shared_ptr<DataBinFile<glm::vec3>> fileDataBuffer, DataVector<glm::vec3>& triVertexData, DataVector<Face>& faceDataBuffer)
{
	DataVector<glm::vec3> triVertexBuffer(POINT);
	faceIndexTriangulate.Process(triVertexBuffer, triVertexData, faceDataBuffer);

	DataVector<glm::vec3> triNormalsBuffer = calcnormals.ProcessVec3(triVertexBuffer);

	fileDataBuffer->StartWrite();
	for (int i = 0; i < triVertexBuffer.GetSize(); i++)
	{
		fileDataBuffer->SetData(triVertexBuffer.GetData(i));
		fileDataBuffer->SetData(triNormalsBuffer.GetData(i));
	}

	fileDataBuffer->SaveFile();
	fileDataBuffer->CloseFile();
}

void ZoneGenerator::loadSubVisDataOffset(shared_ptr<DataBinFile<glm::vec3>> fileDataBuffer, 
											DataVector<glm::vec3>& triVertexData, 
											DataVector<Face>& faceDataBuffer, 
											glm::vec3 offset)
{
	DataVector<glm::vec3> triVertexBuffer(POINT);
	faceIndexTriangulate.Process_Offset(triVertexBuffer, triVertexData, faceDataBuffer, offset);

	DataVector<glm::vec3> triNormalsBuffer = calcnormals.ProcessVec3(triVertexBuffer);

	fileDataBuffer->StartWrite();
	for (int i = 0; i < triVertexBuffer.GetSize(); i++)
	{
		fileDataBuffer->SetData(triVertexBuffer.GetData(i));
		fileDataBuffer->SetData(triNormalsBuffer.GetData(i));
	}

	fileDataBuffer->SaveFile();
	fileDataBuffer->CloseFile();
}