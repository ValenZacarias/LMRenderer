#include "PATriangulate.h"
#include "DataVectorTest.h"
#include <glm/glm.hpp> 

PATriangulate::PATriangulate()
{
}
//Aca tendriamos una sobrecarga de process por tipo de dato de retorno?
//Es decir, si queremos mandarlo a un Data que pueda escribir a archivo de texto, sobrecargamos con un retorno de DataTextFile
DataVectorTest<float> PATriangulate::Process(std::shared_ptr<DataVectorTest<int>> elementIndx, int perElemIndex)
{											//Aca deberiamos poder generalizar al DATA que use la visualizacion para tomar sus indices
	//Debariamos hacer un reserve aca, asi no resizeamos multiples veces
	DataVectorTest<float> trisIndx = DataVectorTest<float>(FLOATVAL);

	for (int i = 0; i < elementIndx->GetSize(); i += perElemIndex) //Agarro el quad completo
	{
		//Tri 1
		trisIndx.SetData(elementIndx->GetData(i));
		trisIndx.SetData(elementIndx->GetData(i + 1));
		trisIndx.SetData(elementIndx->GetData(i + 2));
		//Tri 2
		trisIndx.SetData(elementIndx->GetData(i + 2));
		trisIndx.SetData(elementIndx->GetData(i + 3));
		trisIndx.SetData(elementIndx->GetData(i));
	}

	return trisIndx;
}


DataVectorTest<float> PATriangulate::Process(std::shared_ptr<DataVectorTest<float>> vertex, 
												std::shared_ptr<DataVectorTest<int>> elementIndx, 
												int perElemIndex)
{											//Aca deberiamos poder generalizar al DATA que use la visualizacion para tomar sus indices
	//Debariamos hacer un reserve aca, asi no resizeamos multiples veces
	DataVectorTest<float> trisVertex = DataVectorTest<float>(FLOATVAL);

	int t1_0;
	int t1_1;
	int t1_2;

	int t2_0;
	int t2_1;
	int t2_2;

	//Refactorizar de forma que use el structs de Polygons
	for (int i = 0; i < elementIndx->GetSize(); i += perElemIndex) //Agarro el quad completo
	{
		//Tri 1 (3 vertex)
		// 3 * debido a la cantidad de floats por vertice
		t1_0 = elementIndx->GetData(i);
		t1_1 = elementIndx->GetData(i + 1);
		t1_2 = elementIndx->GetData(i + 2);
		
		trisVertex.SetData(vertex->GetData(3*t1_0));
		trisVertex.SetData(vertex->GetData(3*t1_0 + 1));
		trisVertex.SetData(vertex->GetData(3*t1_0 + 2));
						   
		trisVertex.SetData(vertex->GetData(3*t1_1));
		trisVertex.SetData(vertex->GetData(3*t1_1 + 1));
		trisVertex.SetData(vertex->GetData(3*t1_1 + 2));
						   
		trisVertex.SetData(vertex->GetData(3*t1_2));
		trisVertex.SetData(vertex->GetData(3*t1_2 + 1));
		trisVertex.SetData(vertex->GetData(3*t1_2 + 2));

		t2_0 = elementIndx->GetData(i + 2);
		t2_1 = elementIndx->GetData(i + 3);
		t2_2 = elementIndx->GetData(i);

		trisVertex.SetData(vertex->GetData(3*t2_0));
		trisVertex.SetData(vertex->GetData(3*t2_0 + 1));
		trisVertex.SetData(vertex->GetData(3*t2_0 + 2));
							
		trisVertex.SetData(vertex->GetData(3*t2_1));
		trisVertex.SetData(vertex->GetData(3*t2_1 + 1));
		trisVertex.SetData(vertex->GetData(3*t2_1 + 2));
							
		trisVertex.SetData(vertex->GetData(3*t2_2));
		trisVertex.SetData(vertex->GetData(3*t2_2 + 1));
		trisVertex.SetData(vertex->GetData(3*t2_2 + 2));
	}

	//DEBUG
	/*for (int i = 0; i < trisVertex.GetSize(); i++)
	{
		if(i%3 == 0) cout << " " << endl;
		std::cout << trisVertex.GetData(i) << ", ";
	}*/

	return trisVertex;
}