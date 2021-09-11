#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "VisBase.h"
#include "VisGroup.h"
#include "VisPrimitive.h"
#include "GLFWCanvas.h"

VisGroup::VisGroup() {};
VisGroup::~VisGroup() 
{
	//for (int i = 0; i < visualizations.size(); i++)
	//{
	//	delete visualizations[i]; //Recursion
	//}
};

void VisGroup::Render(Camera* cam)
{
	for (int i = 0; i < visualizations.size(); i++)
	{
		visualizations[i]->Render(cam); //Recursion
	}
}