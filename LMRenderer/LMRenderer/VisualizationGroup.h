#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"

class VisualizationGroup : public VisualizationBase
{
private:
public:
	std::vector<VisualizationBase*> visualizations {};
	VisualizationGroup();
	~VisualizationGroup();
	void Render(Camera* cam) override;
};