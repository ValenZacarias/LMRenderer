#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "VisualizationBase.h"
#include "VisualizationGrid.h"
#include "GLFWCanvas.h"

VisualizationGrid::VisualizationGrid()
{
    Shader lightShader("light_vertex_shader.txt", "light_fragment_shader.txt");
    this->shader = lightShader;
	GenerateBuffers();
}

void VisualizationGrid::Render(Camera* cam)
{
	glBindVertexArray(VAO);
    shader.use();
    shader.setVec3("lightColor", 0.9f, 0.9f, 0.9f);
    
    glm::mat4 view;
    view = glm::lookAt(cam->cameraPos, cam->cameraPos + cam->cameraFront, cam->cameraUp);
    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(cam->FOV), 1280.0f / 720.0f, 1.0f, 1000.0f);
    unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_LINES, lenght, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

int VisualizationGrid::GenerateBuffers()
{
    int slices = 20;
    float size = 100;
    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec4> indices;

    for (int j = 0; j <= slices; ++j) {
        for (int i = 0; i <= slices; ++i) {
            float x = (float)i / (float)slices;
            float y = 0;
            float z = (float)j / (float)slices;
            vertices.push_back(glm::vec3(x * size, y * size, z * size) - glm::vec3(size/2, 0.0f, size/2));
        }
    }

    for (int j = 0; j < slices; ++j) {
        for (int i = 0; i < slices; ++i) {

            int row1 = j * (slices + 1);
            int row2 = (j + 1) * (slices + 1);

            indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
            indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));

        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    lenght = (GLuint)indices.size() * 4;
	return 0;
}
