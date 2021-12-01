#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "shader_s.h"

#include "GLFWCanvas.h"

using namespace std;
using namespace glm;

//struct Camera;

class DrawLine 
{
    Shader shader;
    GLuint VBO, VAO;
    vector<float> vertices;
    vec3 startPoint;
    vec3 endPoint;
    vec3 lineColor;
    float lineWidth;

public:
    DrawLine() {};
    DrawLine(vec3 start, vec3 end, float width, vec3 color = vec3(1.0f, 0.0f, 1.0f))
    {
        startPoint = start;
        endPoint = end;
        lineColor = color;
        lineWidth = width;

        Shader lineShader("light_vertex_shader.txt", "light_fragment_shader.txt");
        this->shader = lineShader;

        vertices = 
        {
             start.x, start.y, start.z,
             end.x, end.y, end.z,
        };

        GenerateBuffers();
    }

    void GenerateBuffers()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Render(Camera* cam) 
    {
        shader.use();
        shader.setVec3("lightColor", lineColor.r, lineColor.g, lineColor.b);

        unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view_matrix));

        glm::mat4 projection;
        if (PERSPECTIVE_CAM)
            projection = cam->perspective_matrix;
        else
            projection = cam->ortho_matrix;

        unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model = glm::mat4(1.0f * MESH_SCALE);
        unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glLineWidth(lineWidth);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
    }

    ~DrawLine() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};