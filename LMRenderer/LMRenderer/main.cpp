//https://github.com/JoeyDeVries/LearnOpenGL
//Un cambio de prueba

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "Globals.h"
#include "Tool.h"
#include "ToolFPSCamera.h"
#include "GLFWCanvas.h"

#include "VisualizationBase.h"
#include "VisualizationGroup.h"
#include "VisualizationPrimitive.h"
#include "VisualizationPrimitiveTest.h"

int screenWidth = 1280;
int screenHeigth = 720;

float lastFrame = 0.0f;

GLFWwindow* window = NULL;

int main()
{

	GLFWCanvas canvas = GLFWCanvas(1280, 720);
	window = canvas.Init();

	//Ahora le damos un ptr a algo que esta en el stack y muere con el main
	//Si la idea es ir creando y destruyendo a medida que cambiamos, podemos usar smart pointers
	ToolFPSCamera cameraTool = ToolFPSCamera(&canvas.currentCamera);
	canvas.SetCurrentTool(&cameraTool);
	

	glEnable(GL_DEPTH_TEST);

	Shader lightShader("light_vertex_shader.txt", "light_fragment_shader.txt");

	//Cube with Normals
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	GLfloat pyramidVertices[] = {
		// first triangle texture
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f, 1.0f,//1.0f, 1.0f, // upper right corner
		0.5f, -0.5f, 0.5f,   1.0f,  1.0f, 1.0f,//1.0f, 0.0f, // lower right corner
		-0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 1.0f, //0.0f, 1.0f, // upper left corner
		// second triangle
		0.5f, -0.5f, 0.5f,  1.0f,  1.0f, 1.0f,//1.0f, 0.0f, // lower right corner
		-0.5f, -0.5f, 0.5f, 1.0f,  1.0f, 1.0f,//0.0f, 0.0f, // lower left corner
		-0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 1.0f,//0.0f, 1.0f, // upper left corner
		//The third triangle
	   -0.5f, -0.5f, 0.5f, 1.0f,  1.0f, 1.0f,//0.0f, 0.0f, // lower left corner
		0.0f, 0.4f, 0.0f,  1.0f,  1.0f, 1.0f,// 0.5f, 1.0f, // vertex
		0.5f, -0.5f, 0.5f, 1.0f,  1.0f, 1.0f,// 1.0f, 0.0f, // lower right corner
		//fourth triangle
		0.5f, -0.5f, 0.5f, 1.0f,  1.0f, 1.0f,// 0.0f, 0.0f, // lower right corner
		0.0f, 0.4f, 0.0f,  1.0f,  1.0f, 1.0f,//0.5f, 1.0f, // vertex
		0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 1.0f,//1.0f, 0.0f, // upper right corner
		//The fifth triangle
		0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 1.0f,//0.0f, 0.0f, // upper right corner
		0.0f, 0.4f, 0.0f,   1.0f,  1.0f, 1.0f,// 0.5f, 1.0f, // vertex
		-0.5f, -0.5f, -0.5f,1.0f,  1.0f, 1.0f,// 1.0f, 0.0f, // upper left corner
		// sixth triangle
		-0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 1.0f,//0.0f, 0.0f, // upper left corner
		0.0f, 0.4f, 0.0f,    1.0f,  1.0f, 1.0f,//0.5f, 1.0f, // vertex
		-0.5f, -0.5f, 0.5f,  1.0f,  1.0f, 1.0f,// 1.0f, 0.0f // lower left corner
	};

	// world space positions of our cubes
	//glm::vec3 cubePositions[] = {
	//	glm::vec3(0.0f,  0.0f,  0.0f),
	//	glm::vec3(2.0f,  5.0f, 0.0f),
	//	glm::vec3(-1.5f, -2.2f, -2.5f),
	//	glm::vec3(-3.8f, -2.0f, -12.3f),
	//	glm::vec3(2.4f, -0.4f, -3.5f),
	//	glm::vec3(-1.7f,  3.0f, -7.5f),
	//	glm::vec3(1.3f, -2.0f, -2.5f),
	//	glm::vec3(1.5f,  2.0f, -2.5f),
	//	glm::vec3(1.5f,  0.2f, -1.5f),
	//	glm::vec3(-1.3f,  1.0f, -1.5f)
	//};

	//glm::vec3 lightPosition = glm::vec3(0.0f, 2.0f, 0.0f);

	//Vertex buffer def ---------------------------------------------------------------------
	GLuint cubeVBO, cubeVAO;

	//Vertex array object init
	glGenVertexArrays(1, &cubeVAO);

	//Vertex buffer data initialize
	glGenBuffers(1, &cubeVBO);

	//Pass vertex data to buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	//Position Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	////Normal Pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint pyramidVBO, pyramidVAO;

	//Vertex array object init
	glGenVertexArrays(1, &pyramidVAO);

	//Vertex buffer data initialize
	glGenBuffers(1, &pyramidVBO);

	//Pass vertex data to buffer
	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

	glBindVertexArray(pyramidVAO);

	//Position Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	////Normal Pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Light vertex buffer def ---------------------------------------------------------------
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//Pass vertex data to buffer
	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);*/

	//Position Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//DATA END ----------------------------------------------------------------------
	
	//Armado de Visualizations
	VisualizationPrimitive CubeViz = VisualizationPrimitive(&pyramidVAO, &pyramidVBO);
	VisualizationPrimitiveTest CubeViz2 = VisualizationPrimitiveTest(&cubeVAO, &cubeVBO);

	VisualizationGroup MainGroupViz = VisualizationGroup();
	VisualizationGroup TestGroupViz = VisualizationGroup();

	TestGroupViz.visualizations.insert(TestGroupViz.visualizations.begin(), &CubeViz2);

	MainGroupViz.visualizations.insert(MainGroupViz.visualizations.begin(), &CubeViz);
	MainGroupViz.visualizations.insert(MainGroupViz.visualizations.begin() + 1, &TestGroupViz);
	//MainGroupViz.visualizations[0] = CubeViz;

	canvas.SetupContext(&MainGroupViz);//SMART POINTER
	// DEBEMOS BUSCAR UNA FORMA DE SABER QUIEN ES DUEÑO DE QUE, A CANVAS LE PASAN LA VISUALIZACION, DE LA CUAL ES DUEÑA
	// LA CLASE MAIN. DEBEMOS REGISTRAR LA INTENCION DE CADA PUNTERO

	float currentFrame = 0.0f;
	//RENDER LOOP! -----------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		canvas.KeyboardHandler(window);
		canvas.Render();

		//3D Transformations --------------------------------------------------------------------------------------
		//view: World -> View
		glm::mat4 view;
		view = glm::lookAt(canvas.currentCamera.cameraPos, canvas.currentCamera.cameraPos + canvas.currentCamera.cameraFront, 
							canvas.currentCamera.cameraUp);

		//projection: View -> Clip
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(canvas.currentCamera.FOV), (float)screenWidth / (float)screenHeigth, 1.0f, 1000.0f);

		//Render light --------------------------------------------------------------------------------------
		lightShader.use();
		lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		unsigned int lightViewLoc = glGetUniformLocation(lightShader.ID, "view");
		glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int lightProjectionLoc = glGetUniformLocation(lightShader.ID, "projection");
		glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.3f));
		unsigned int modelLoc = glGetUniformLocation(lightShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//End render --------------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &pyramidVAO);
	glDeleteBuffers(1, &pyramidVBO);
	glDeleteVertexArrays(1, &lightVAO);

	glfwTerminate();
	return 0;
}


