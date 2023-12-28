#ifndef GAME_H
#define GAME_H
#include <map>
#include <string>
#include <queue>
#include <set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/GraphLine.hpp"
#include "../include/model.hpp"
#include "../include/map.hpp"
#include "../include/Particle.hpp"
#include "../include/PhysicalEngine.h"

struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback {
    bool m_connected;

    MyContactResultCallback() : m_connected(false) {}

    virtual btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0, int partId0, int index0,
        const btCollisionObjectWrapper* colObj1, int partId1, int index1)
    {
        if (cp.getDistance() < 0.f) { // 检查穿透深度，小于0表示物体相互接触
            m_connected = true;
        }
        return 0; // 继续检测其他接触点
    }
};

class Game{
public:
    GLuint W_width, W_height;
    //std::map<std::string, Model*> Mdl;
    Model* dragon1;
    Model* dragon2;
    Model* dragon3;
    Model* dragon_jump;
    Model* dragon_lay;
    std::vector<Model*> Obstacle_q;
    std::vector<Model*> Obstacle_pool;
    std::vector<Model*> obstacle_Engine;
    ParticleGenerator* Particles;
    std::set<int> useObstacle;
    const int maxObstacle = 40;
    const float deltaTime = 0.1;
    myBulletEngine btEngine;
    float jumpV;
    int prob = 0;
    int prob2 = 0;
    int prob3 = 0;
    bool jumpLock = false;
    bool lowHeadLock = false;
    bool connected = true;    

    Model* xianrenzhang;
    Model* yilong;

    bool END = false;
public:
    Game(GLuint width, GLuint height);
    Game();
    ~Game();
    void jump();
    void lowHead(bool press);
    void init();
    void ProcessInput();
    void Update(int cnt);
};

Game::Game(){

}

Game::Game(GLuint width, GLuint height){

}

Game::~Game(){
    for(auto &x : Obstacle_q){
        delete x;
    }
    delete dragon1;
    delete dragon2;
    delete dragon3;
    delete dragon_jump;
}


void Game::init(){
    ResourceManager::LoadShader("../shader/dragonShader.vs", "../shader/dragonShader.fs", nullptr, "dragon");
    ResourceManager::LoadShader("../shader/particleShader.vs", "../shader/particleShader.fs", nullptr, "particle");
    ResourceManager::LoadTexture("../resources/textures/green.png", false, "green");
    ResourceManager::LoadTexture("../resources/textures/floor2.png", false, "particle");

    btEngine.btInit();
    btEngine.addDinosaur(btVector3(0.1f, 0.1f, 0.01f), btVector3(0.0f, 0.8f, 0.0f));
    btEngine.addGround(btVector3(100.0f, 0.65f, 100.0f), btVector3(0.0f, 0.0f, 0.0f));
    Particles = new ParticleGenerator("particle", 50);

    dragon1 = new Model("dino1", glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    dragon2 = new Model("dino2", glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    dragon3 = new Model("dino3", glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    dragon_jump = new Model("dino-jump", glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    dragon_lay = new Model("dino-lay", glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    jumpV = 0;
    dragon1->loadModel();
    dragon2->loadModel();
    dragon3->loadModel();
    dragon_jump->loadModel();
    dragon_lay->loadModel();

    xianrenzhang = new Model("xianren", glm::vec3(60.0f, 0.0f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.1f, 0.5f, 0.1f));
    yilong = new Model("dragon", glm::vec3(60.0f, 2.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.5f, 0.1f, 0.5f));
    xianrenzhang->loadModel();
    yilong->loadModel();
    for (int i = 0; i < 20; i++) {
        Model* new_model = new Model(xianrenzhang);
        Obstacle_pool.push_back(new_model);
    }
    for (int i = 0; i < 20; i++) {
        Model* new_model = new Model(yilong);
        Obstacle_pool.push_back(new_model);
    }
}


void Game::ProcessInput(){
    
}


void Game::jump(){
    MyContactResultCallback callback;
    btEngine.world->contactPairTest(btEngine.ground, btEngine.dinosaur, callback);
    /*std::cout << connected << std::endl;
    btTransform trans;
    btEngine.dinosaur->getMotionState()->getWorldTransform(trans);
    btVector3 pos = trans.getOrigin();
    std::cout << "X:" << pos.x() << "\tY:" << pos.y() << "\tZ:" << pos.z() << std::endl;*/
    connected = callback.m_connected;
    if (connected) {
        btEngine.DinosaurJump(btVector3(0.0f, 15.0f, 0.0f));
        std::cout << "jump!\n";
        jumpLock = true;
    }
}

void Game::lowHead(bool press){
    if(press){
        lowHeadLock = true;
        btTransform currentTransform = btEngine.dinosaur->getWorldTransform();
        
        btQuaternion newRotation;
        newRotation.setEulerZYX(0, 0, 90.0f);
        currentTransform.setRotation(newRotation);

        btEngine.dinosaur->setWorldTransform(currentTransform);

    }
    else{
        lowHeadLock = false;
        btTransform currentTransform = btEngine.dinosaur->getWorldTransform();

        btQuaternion newRotation;
        newRotation.setEulerZYX(0, 0, -90.0f);
        currentTransform.setRotation(newRotation);

        btEngine.dinosaur->setWorldTransform(currentTransform);
    }
}

void Game::Update(int cnt){
    btEngine.world->stepSimulation(1.f / 60.f, 10);

    Particles->Update(1, jumpLock); 
    Particles->Draw();
    if(Obstacle_q.size() < maxObstacle){
        if(rand() % 10000 < prob){
            int index, num = 500;
            while(num--){
                // std::cout<<"ok\n";
                index = rand() % maxObstacle;
                if(useObstacle.count(index) == 0){
                    useObstacle.insert(index);
                    break;
                }
            }
            if(num){
                Obstacle_q.push_back(Obstacle_pool[index]);
                Obstacle_q[Obstacle_q.size() - 1]->setID(index);
                int r = rand() % 4;
                glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
                float height = 2.5f;
                switch (r)
                {
                case 0:
                    pos.y = 0;
                    pos.z = -10;
                    break;
                case 1:
                    pos.y = 0;
                    pos.z = -10;
                    height = 1.2f;
                    break;
                case 2:
                    pos.y = 0.4;
                    pos.z = -15;
                    break;
                case 3:
                    pos.y = 0.8;
                    pos.z = -20;
                    height = 4.0f;
                    break;
                default:
                    break;
                }
                if(Obstacle_q[Obstacle_q.size() - 1]->name == "xianren")
                {
                    Obstacle_q[Obstacle_q.size() - 1]->init(glm::vec3(60.0f, pos.y, pos.z), glm::vec3(0.03f, 0.03f, 0.03f));
                }
                else if (Obstacle_q[Obstacle_q.size() - 1]->name == "dragon")
                {
                    Obstacle_q[Obstacle_q.size() - 1]->init(glm::vec3(60.0f, height, -10.0f), glm::vec3(0.3f, 0.3f, 0.3f));
                } 
            }

                      
            prob = 0;
        }
        else{
            prob++;
        }
    }


    /****************************************************************/
    // 这是属于恐龙跑道上的障碍物，运用物理引擎
    /****************************************************************/
    if (obstacle_Engine.size() < maxObstacle) {
        if (rand() % 10000 < prob2) {
            Model* new_model;
            if (rand() % 100 < 50) {
                new_model = new Model(xianrenzhang);

                obstacle_Engine.push_back(new_model);
                std::cout << obstacle_Engine.size() << std::endl;

                // 物理世界同步生成障碍物
                btVector3 shape(0.1f, 0.3f, 0.1f);
                btVector3 position(60.0f, 0.2f, 0.0f);
                btVector3 speed(0.0f, 0.0f, 0.0f);
                float mass = 0.2f;
                bool fly = false;

                btEngine.addBarrier(shape, position, mass, speed, fly);

                // 障碍物添加到useBarriers（此步骤是由前面代码的问题产生的多余步骤）
                btEngine.useBarriers.push_back(btEngine.barriers[btEngine.barriers.size() - 1]);
            }
            else {
                new_model = new Model(yilong);

                obstacle_Engine.push_back(new_model);
                std::cout << obstacle_Engine.size() << std::endl;

                // 物理世界同步生成障碍物
                btVector3 shape(0.3f, 0.05f, 0.05f);
                btVector3 position(60.0f, 1.0f, 0.0f);
                switch (rand() % 3) {
                case 0:
                    position.setY(0.9f);
                    break;
                case 1:
                    position.setY(2.0f);
                    break;
                case 2:
                    position.setY(4.0f);
                    break;
                default:
                    break;
                }
                btVector3 speed(0.0f, 0.0f, 0.0f);
                float mass = 0.2f;
                bool fly = true;

                btEngine.addBarrier(shape, position, mass, speed, fly);

                // 障碍物添加到useBarriers（此步骤是由前面代码的问题产生的多余步骤）
                btEngine.useBarriers.push_back(btEngine.barriers[btEngine.barriers.size() - 1]);
            }
            prob2 = 0;
        }
        else {
            prob2++;
        }
    }

    /****************************************************************/
    // 设置速度
    /****************************************************************/
    for (auto& x : btEngine.useBarriers) {
        x->barrier->activate();
        x->barrier->setLinearVelocity(btVector3(-5.0f, 0.0f, 0.0f));
        
        MyContactResultCallback callback;
        btEngine.world->contactPairTest(x->barrier, btEngine.dinosaur, callback);
        connected = callback.m_connected;
        if (connected)
        {
            END = true;
            std::cout << "Game Over!!!" << '\n';
            exit(0);
        }
        //x->setyz0();
        if (x->fly)
            x->barrier->setGravity(btVector3(0, 0, 0));
    }
    /****************************************************************/
    // sin函数飞舞
    /****************************************************************/
    //float amplitude = 0.2, frequency = 1.0;
    //btEngine.CloudsFly(deltaTime, amplitude, frequency);
    /****************************************************************/
    // render障碍物
    /****************************************************************/
    bool removeBLT = false;
    for (int i = 0; i < obstacle_Engine.size(); i++) {
        obstacle_Engine[i]->render(btEngine.getBarriersMode(i));
        if (i >= 15) {
            removeBLT = true;
        }
    }
    /****************************************************************/
    // 清除障碍物
    /****************************************************************/
    if (removeBLT) {
        delete obstacle_Engine[0];
        delete btEngine.barriers[0];
        //delete btEngine.useBarriers[0];
        obstacle_Engine.erase(obstacle_Engine.begin());
        btEngine.barriers.erase(btEngine.barriers.begin());
        btEngine.useBarriers.erase(btEngine.useBarriers.begin());
    }

    /****************************************************************/
    // Bullet Engine End
    /****************************************************************/


    bool remove = false;

    for(auto &x : Obstacle_q){
        x->update(deltaTime);
        if(x->pos.x < -3){
            remove = true;
        }
    }
    if (jumpLock) {
        dragon_jump->render(btEngine.getDinosaurMode());
        MyContactResultCallback callback;
        btEngine.world->contactPairTest(btEngine.ground, btEngine.dinosaur, callback);
        if (callback.m_connected)
            jumpLock = false;
    }
    else if (lowHeadLock) {
        dragon_lay->render(btEngine.getDinosaurMode());
    }
    else{
        switch (cnt)
        {
        case 0:
        case 1:
            dragon1->render(btEngine.getDinosaurMode());
            break;
        case 2:
        case 3:
            dragon2->render(btEngine.getDinosaurMode());
            break;
        case 4:
        case 5:
            dragon3->render(btEngine.getDinosaurMode());
            break;
        default:
            break;
        }
    }


    for(auto &x : Obstacle_q){
        x->render();
        if(x->pos.x < -3){
            remove = true;
        }
    }
    if(remove){
        int id = Obstacle_q[0]->ID;
        Obstacle_q.erase(Obstacle_q.begin());
        useObstacle.erase(id);
    }

}

#endif