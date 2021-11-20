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
	//bool forceFileCreation = true;

	//int subVisPerZone = 1;

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

	// First sample
	auto faceDataBuffer = make_shared<DataVector<Face>>(FACE);
	auto cellDataSample = make_shared<DataVector<Cell>>(CELL);

	for (int i = 0; i < subVisPerZone; i++)
	{
		uniformSample.Process(cellData, cellDataSample, 0.1 + (0.3 * i));

		reconstructCells.Process(*faceData, *faceDataBuffer, *cellDataSample);

		string filename = "bin/zone_" + to_string(generatedZones) + "_" + to_string(i);

		if (forceFileCreation)
		{
			auto triVertexBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);

			triVertexBuffer->StartWrite();
			faceIndexTriangulate.Process(*triVertexBuffer, *triVertexData, *faceDataBuffer);
			triVertexBuffer->SaveFile();
			triVertexBuffer->CloseFile();

			auto vis = make_shared<VisCell_bin>(triVertexBuffer);
			visZone->AddSubVisualization(vis);
		}
		else
		{
			fstream zonefile(filename);

			auto triVertexBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);

			if (!triVertexBuffer->FileExists())
			{
				triVertexBuffer->StartWrite();
				faceIndexTriangulate.Process(*triVertexBuffer, *triVertexData, *faceDataBuffer);
				triVertexBuffer->SaveFile();
				triVertexBuffer->CloseFile();
			}

			auto vis = make_shared<VisCell_bin>(triVertexBuffer);
			visZone->AddSubVisualization(vis);
		}
	}

	cellDataSample->Clear();
	faceDataBuffer->Clear();

	return visZone;
}

vector<shared_ptr<VisMeshZone>> ZoneGenerator::GenerateRepeatedZones(string meshFilePath, int zoneCount, int columns, int rows)
{
	vector<shared_ptr<VisMeshZone>> zones {};

	int subVisPerZone = 3;
	bool forceFileCreation = false;
	//bool forceFileCreation = true;
	//int subVisPerZone = 1;

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

	//int columns = 5;
	//int rows = 5;

	for (int j = 0; j < zoneCount; j++) // Zone loop
	{
		shared_ptr<VisMeshZone> visZone = make_shared<VisMeshZone>();

		//glm::vec3 offset = x_offset * (float)j;
		glm::vec3 offset = (x_offset * (float)(j % columns)) + z_offset * (float)(trunc(j/ rows));

		vector<glm::vec3> BBOffseted;
		for (int p = 0; p < BBVertices.size(); p++) { BBOffseted.push_back(BBVertices[p] + offset); }
		visZone->SetBoundingBox(BBOffseted);

		for (int i = 0; i < subVisPerZone; i++) // Samplings Loop
		{
			uniformSample.Process(cellData, cellDataSample, 0.1 + (0.3 * i));

			reconstructCells.Process(*faceData, *faceDataBuffer, *cellDataSample);

			string filename = "bin/zone_" + to_string(j) + "_" + to_string(i);

			if (forceFileCreation)
			{
				auto triVertexBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);

				triVertexBuffer->StartWrite();
				faceIndexTriangulate.Process_Offset(*triVertexBuffer, *triVertexData, *faceDataBuffer, offset);
				triVertexBuffer->SaveFile();
				triVertexBuffer->CloseFile();

				auto vis = make_shared<VisCell_bin>(triVertexBuffer);
				visZone->AddSubVisualization(vis);
			}
			else
			{
				fstream zonefile(filename);

				auto triVertexBuffer = make_shared<DataBinFile<glm::vec3>>(POINT, filename);

				if (!triVertexBuffer->FileExists())
				{
					triVertexBuffer->StartWrite();
					faceIndexTriangulate.Process_Offset(*triVertexBuffer, *triVertexData, *faceDataBuffer, offset);
					triVertexBuffer->SaveFile();
					triVertexBuffer->CloseFile();
				}

				auto vis = make_shared<VisCell_bin>(triVertexBuffer);

				visZone->AddSubVisualization(vis);
			}

			cellDataSample->Clear();
			faceDataBuffer->Clear();
			__nop();
		}
		zones.push_back(visZone);
		__nop();
	}
	return zones;
}