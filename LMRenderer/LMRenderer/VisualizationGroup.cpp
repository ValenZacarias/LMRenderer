#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"
#include "VisualizationGroup.h"
#include "VisualizationPrimitive.h"
#include "GLFWCanvas.h"

VisualizationGroup::VisualizationGroup() {};

void VisualizationGroup::Render(Camera* cam, Shader* shader)
{
	//std::cout << "VGROUP RENDER" << std::endl;
	//for (VisualizationBase v : visualizations)
	//{
	//	std::cout << "child..." << std::endl;
	//	v.Render(cam, shader); //Recursion
	//}

	for (int i = 0; i < visualizations.size(); i++)
	{
		//std::cout << "child: " << visualizations[i]->VAO << std::endl;
		visualizations[i]->Render(cam, shader); //Recursion
	}
}