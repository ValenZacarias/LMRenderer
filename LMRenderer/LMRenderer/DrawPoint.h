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

class DrawPoint
{
    Shader shader;
    GLuint VBO, VAO;
    vector<float> vertices;
    vec3 startPoint;
    //vec3 endPoint;
    vec3 pointColor;
    float pointSize;

public:
    DrawPoint() {};
    DrawPoint(vec3 start, float size, vec3 color = vec3(1.0f, 0.0f, 1.0f))
    {

        startPoint = start;
        //endPoint = end;
        pointColor = color;
        pointSize = size;

        Shader pointShader("light_vertex_shader.txt", "light_fragment_shader.txt");
        this->shader = pointShader;

        vertices =
        {
             start.x, start.y, start.z
             //end.x, end.y, end.z,
        };

        GenerateBuffers();
    }

    ~DrawPoint() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        //glDeleteProgram(shaderProgram);
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
        shader.setVec3("lightColor", pointColor.r, pointColor.g, pointColor.b);

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

        //glUseProgram(shaderProgram);
        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        //glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);
        //glLineWidth(lineWidth);
        glPointSize(pointSize);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        //glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, NULL);

        //glBindVertexArray(0);
    }


    void setColor(vec3 color) {
        pointColor = color;
    }
};