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
protected:
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

    Mdl["dragon"] = new Model("cube");

    jumpV = 0;
    for(auto& [name, model]:Mdl){
        model->init();
    }
}


void Game::ProcessInput(){
    
}


void Game::jump(){
    if(!jumpLock){
        jumpV = 6;
        jumpLock = true;
    }
}

void Game::lowHead(bool press){
    if(press){
        Mdl["dragon"]->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    }
    else{
        Mdl["dragon"]->setScale(glm::vec3(0.5f, 0.8f, 0.5f));
    }
}

void Game::Update(){
    if(Obstacle_q.size() < maxObstacle){
        if(rand() % 10000 < prob){
            if(rand() % 10 < 5)
                Obstacle_q.push_back(new Model("cube", glm::vec3(10.0f, 0.5f, 0.0f), glm::vec3(0.1f, 0.5f, 0.1f)));
            else
                Obstacle_q.push_back(new Model("cube", glm::vec3(10.0f, 2.0f, 0.0f), glm::vec3(0.5f, 0.1f, 0.5f)));
            Obstacle_q[Obstacle_q.size() - 1]->init();
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
                float v = jumpV - 3;
                model->pos.y += v * deltaTime;
                jumpV -= deltaTime * 5;
                for (auto &x : Obstacle_q)
                {
                    x->pos.x -= 0.3 * deltaTime;
                    if (x->pos.x < -3)
                        remove = true;
                }
                if(jumpV < 0)
                    jumpLock = false;
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