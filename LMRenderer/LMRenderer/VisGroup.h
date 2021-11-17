#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "VisBase.h"

using namespace std;

class VisGroup : public VisBase
{
private:
public:
	std::vector<VisBase*> visualizations {};
	std::vector<shared_ptr<VisBase>> shared_visualizations {};
	VisGroup();
	~VisGroup();
	void Render(Camera* cam) override;
};