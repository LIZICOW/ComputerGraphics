#ifndef MAP_H
#define MAP_H
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

#include "../include/manResource.hpp"

class Map
{
protected:
    GLuint VAO, VBO;
    GLfloat centerX, centerY, centerZ;
    GLfloat L, W; //长宽
    std::vector<GLfloat> points;

public:
    Map();
    void init();
    void render();
    ~Map();
};

Map::Map(/* args */)
{
    centerX = centerY = centerZ = 0;
    L = 25;
    W = 2.5;
    ResourceManager::LoadShader("../shader/mapShader.vs", "../shader/mapShader.fs", nullptr, "map");
    ResourceManager::GetShader("map").use().setVec3("color", glm::vec3(1.0f, 1.0f, 0.2f));
}

void Map::init(){
    points.push_back(centerX + L);
    points.push_back(centerY);
    points.push_back(centerZ + W);

    points.push_back(centerX - L);
    points.push_back(centerY);
    points.push_back(centerZ + W);

    points.push_back(centerX - L);
    points.push_back(centerY);
    points.push_back(centerZ - W);

    points.push_back(centerX - L);
    points.push_back(centerY );
    points.push_back(centerZ - W);

    points.push_back(centerX + L);
    points.push_back(centerY);
    points.push_back(centerZ - W);

    points.push_back(centerX + L);
    points.push_back(centerY);
    points.push_back(centerZ + W);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Map::render(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, points.size() / 3);
}

Map::~Map()
{
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


#endif