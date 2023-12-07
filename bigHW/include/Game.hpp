#ifndef GAME_H
#define GAME_H
#include <map>
#include <string>
#include <queue>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/GraphLine.hpp"
#include "../include/model.hpp"
#include "../include/map.hpp"

enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
};

class Game{
public:
    GLuint W_width, W_height;
    std::map<std::string, Model*> Mdl;
    std::vector<Model*> Obstacle_q;
    const int maxObstacle = 5;
    const float deltaTime = 0.05;
    float jumpV;
    int prob = 0;
    bool jumpLock = false;

public:
    GameState state;
    Game(GLuint width, GLuint height);
    Game();
    ~Game();
    void jump();
    void lowHead(bool press);
    void init();
    void ProcessInput();
    void Update();
};

Game::Game(){

}

Game::Game(GLuint width, GLuint height){

}

Game::~Game(){
    for(auto &x : Obstacle_q){
        delete x;
    }
    for(auto& [name, model]:Mdl){
        delete model;
    }
}


void Game::init(){
    ResourceManager::LoadShader("../shader/dragonShader.vs", "../shader/dragonShader.fs", nullptr, "dragon");
    ResourceManager::LoadTexture("../resources/textures/green.png", false, "green");

    Mdl["dragon"] = new Model("dino", glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    jumpV = 0;
    Mdl["dragon"]->loadModel();
    // for(auto& [name, model]:Mdl){
    //     model->loadModel();
    // }
}


void Game::ProcessInput(){
    
}


void Game::jump(){
    if(!jumpLock){
        jumpV = 8;
        jumpLock = true;
    }
}

void Game::lowHead(bool press){
    if(press){
        Mdl["dragon"]->setScale(glm::vec3(0.02f, 0.007f, 0.01f));
        Mdl["dragon"]->setPos(glm::vec3(0.0f, 0.6f, 0.0f));
    }
    else{
        Mdl["dragon"]->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
        Mdl["dragon"]->setPos(glm::vec3(0.0f, 0.8f, 0.0f));
    }
}

void Game::Update(){
    if(Obstacle_q.size() < maxObstacle){
        if(rand() % 10000 < prob){
            if(rand() % 10 < 5)
                Obstacle_q.push_back(new Model("xianren", glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.04f, 0.04f, 0.04f), glm::vec3(0.1f, 0.5f, 0.1f)));
            else
                Obstacle_q.push_back(new Model("chahu2", glm::vec3(10.0f, 2.0f, 0.0f), glm::vec3(0.5f, 0.1f, 0.5f), glm::vec3(0.5f, 0.1f, 0.5f)));
            Obstacle_q[Obstacle_q.size() - 1]->loadModel();
            prob = 0;
        }
        else{
            prob++;
        }
    }
    bool remove = false;

    for(auto &x : Obstacle_q){
        x->update(deltaTime);
        if(x->pos.x < -3){
            remove = true;
        }
    }
    for(auto& [name, model]:Mdl){
        if(name == "dragon"){
            if(jumpLock){
                float v = jumpV - 4;
                model->pos.y += v * deltaTime;
                jumpV -= deltaTime * 5;
                for (auto &x : Obstacle_q)
                {
                    x->pos.x -= 0.4 * deltaTime;
                    if (x->pos.x < -3)
                        remove = true;
                }
                if(jumpV < 0)
                    jumpLock = false;
            }
            if( Obstacle_q.size() > 0 &&
                model->collision.x + model->pos.x > Obstacle_q[0]->collision.x + Obstacle_q[0]->pos.x &&
                model->pos.x - model->collision.y < Obstacle_q[0]->pos.y + Obstacle_q[0]->collision.y
            ){
                // std::cout<<"collision!!!"<<rand()<<"\n";
            }


            model->render();
            continue;
        }
            
        model->update(deltaTime * 2);
        model->render();
    }

    for(auto &x : Obstacle_q){
        x->render();
        if(x->pos.x < -3){
            remove = true;
        }
    }
    if(remove){
        delete Obstacle_q[0];
        Obstacle_q.erase(Obstacle_q.begin());
    }
}

#endif