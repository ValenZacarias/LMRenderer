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

//void VisualizationGroup::Render(Camera* cam, Shader* shader)
void VisualizationGroup::Render(Camera* cam)
{

	for (int i = 0; i < visualizations.size(); i++)
	{
		visualizations[i]->Render(cam); //Recursion
	}
}