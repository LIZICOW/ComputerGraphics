#ifndef GRAPHLINE_H
#define GRAPHLINE_H
#include <glad/glad.h>
#include <vector>

class Graph{
public:
    GLuint VAO, VBO;
    GLuint vexnum;
    Graph(){

    }
    void Init(){
        GLfloat vertices[] = {
            0.0f, 100.0f, 0.0f,
            0.0f, -100.0f, 0.0f,
            1.0f, 100.0f, 0.0f,
            1.0f, -100.0f, 0.0f
        };
        std::vector<GLfloat> points;
        for (int i = -20; i <= 20; ++i)
        {
            points.push_back(float(i));
            points.push_back(-100.f);
            points.push_back(0.0f);
            points.push_back(1.0f);
            points.push_back(0.0f);
            points.push_back(0.0f);
            points.push_back(float(i));
            points.push_back(100.f);
            points.push_back(0.0f);
            points.push_back(1.0f);
            points.push_back(0.0f);
            points.push_back(0.0f);
        }
        for (int i = -20; i <= 20; ++i)
        {
            points.push_back(-100.f);
            points.push_back(float(i));
            points.push_back(0.0f);
            points.push_back(0.0f);
            points.push_back(1.0f);
            points.push_back(0.0f);
            points.push_back(100.f);
            points.push_back(float(i));
            points.push_back(0.0f);
            points.push_back(0.0f);
            points.push_back(1.0f);
            points.push_back(0.0f);
        }

        points.push_back(0.0f);
        points.push_back(0.0f);
        points.push_back(100.f);
        points.push_back(0.0f);
        points.push_back(0.0f);
        points.push_back(1.0f);
        vexnum = points.size() / 2;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(sizeof(GLfloat)*3));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};


#endif