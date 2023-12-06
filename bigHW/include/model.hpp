#ifndef MODEL_H
#define MODEL_H
#include "manResource.hpp"
#include "../include/camera.h"
#include <string>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
extern Camera camera;
class Model
{
public:
    GLuint VAO, VBO;
    std::string name;
    glm::vec3 pos, scale;
    std::vector<GLfloat> points;
    const char* dirpath = "../resources/models/";
public:
    Model(std::string n, glm::vec3 pos, glm::vec3 scale);
    Model();
    void init();
    void render();
    void update(float deltaTime);
    void setScale(glm::vec3 scale);
    ~Model();
};

Model::Model(){

}

Model::Model(std::string n, glm::vec3 pos = glm::vec3(0.0f, 0.4f, 0.0f), glm::vec3 scale = glm::vec3(0.5f, 0.8f, 0.5f)){
    this->name = n;
    this->pos = pos;
    this->scale = scale;
}

void Model::init(){
/*    std::string path = dirpath + this->name + "_outline.txt";
    std::ifstream in(path);
    if(!in.is_open()){
        std::cout<<"can not open "<<this->name<<" model file: "<<path<<'\n';
        return;
    }
    float x, y, x_old, y_old;
    in >> x_old >> y_old;
    while(in >> x >> y){
        //two triangles
        points.push_back(x_old);
        points.push_back(y_old);
        points.push_back(-0.5);

        points.push_back(x_old);
        points.push_back(y_old);
        points.push_back(0.5);

        points.push_back(x);
        points.push_back(y);
        points.push_back(0.5);


        points.push_back(x);
        points.push_back(y);
        points.push_back(-0.5);

        points.push_back(x);
        points.push_back(y);
        points.push_back(0.5);

        points.push_back(x_old);
        points.push_back(y_old);
        points.push_back(-0.5);

        x_old = x;
        y_old = y;
    }
    in.close();

    path = dirpath + this->name + "_cover.txt";
    in.open(path);
    if(!in.is_open()){
        std::cout<<"can not open "<<this->name<<" model file: "<<path<<'\n';
        return;
    }

    float xul, yul, xur, yur;
    float xdl, ydl, xdr, ydr;
    while(
        in >> xul >> yul
           >> xur >> yur
           >> xdl >> ydl
           >> xdr >> ydr
    ){
        points.push_back(xul);
        points.push_back(yul);
        points.push_back(0.5);

        points.push_back(xur);
        points.push_back(yur);
        points.push_back(0.5);

        points.push_back(xdl);
        points.push_back(ydl);
        points.push_back(0.5);

        points.push_back(xdl);
        points.push_back(ydl);
        points.push_back(0.5);

        points.push_back(xdr);
        points.push_back(ydr);
        points.push_back(0.5);

        points.push_back(xur);
        points.push_back(yur);
        points.push_back(0.5);
    }
*/
    std::string path = dirpath + this->name + ".txt";
    std::ifstream in(path);
    if(!in.is_open()){
        std::cout<<"can not open "<<this->name<<" model file: "<<path<<'\n';
        return;
    }
    GLfloat x,y,z;
    while(in>>x>>y>>z){
        points.push_back(x);
        points.push_back(y);
        points.push_back(z);
    }

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

void Model::render(){
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)WINDOW_W / (float)WINDOW_H,
                                            0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    model = glm::scale(model, this->scale);
    // dragon 待替换
    ResourceManager::GetShader("dragon").use();    
    ResourceManager::GetShader("dragon").setMat4("view", view);    
    ResourceManager::GetShader("dragon").setMat4("projection", projection);    
    ResourceManager::GetShader("dragon").setMat4("model", model);   
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, points.size() / 3);
    // std::cout<<points.size()<<'\n';
}

void Model::update(float deltaTime){
    pos.x -= deltaTime;
}

void Model::setScale(glm::vec3 scale){
    this->scale = scale;
}

Model::~Model()
{
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



#endif