#ifndef MAP_H
#define MAP_H
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "manResource.hpp"

#define FLOOR_NUM 20
#define WATER_NUM 20
#define BACKGROUND_NUM_X 30
#define BACKGROUND_NUM_Z 5
#define VERTICAL_NUM 8

enum TextureID{
    FLOOR1, FLOOR2, FLOOR3, FLOOR4,
    WATER, BACKGROUND1, BACKGROUND2, VERTICAL
};
const char* TextureName[] = {
    "floor1", "floor2", "floor3", "floor4",
    "water", "background1", "background2", "vertical"
};

static inline float randf(float min, float max, int precision = 1000)
{
    if (min > max) std::swap(min, max);
    float delta = max - min;
    auto i = int(delta * precision);
    return ((float)(rand() % i) / (float)precision) + min;
}

class Floor{
public:
    GLuint VAO, VBO;    
    const std::vector<GLfloat> points = {
        2.5f, 0.0f, 2.5f,   0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -2.5f, 0.0f, 2.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -2.5f, 0.0f, -2.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -2.5f, 0.0f, -2.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        2.5f, 0.0f, -2.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        2.5f, 0.0f, 2.5f,   0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    glm::vec3 pos, scale;
    glm::mat4 rotate;
    int texture;
public:
    Floor(int tex, glm::vec3 p = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 s = glm::vec3(1.0f, 1.0f, 1.0f)):pos(p), scale(s){
        texture = tex;
        rotate = glm::mat4(1.0f);
    }

    void setRotate(glm::mat4 m){
        rotate = m;
    }

    void init(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void render(){
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)WINDOW_W / (float)WINDOW_H,
                                                0.1f, 50.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->pos);
        model = glm::scale(model, this->scale);
        model = model * rotate;
        
        ResourceManager::GetShader("map").use();    
        ResourceManager::GetShader("map").setMat4("view", view);    
        ResourceManager::GetShader("map").setMat4("projection", projection);    
        ResourceManager::GetShader("map").setMat4("model", model); 
        ResourceManager::GetShader("map").setVec3("viewPos", camera.Position); 
        ResourceManager::GetShader("map").setFloat("Distance", calDistance(camera.Position, this->pos)); 
        ResourceManager::GetShader("map").setFloat("maxDistance", 50.0f); 

        ResourceManager::GetTexture(TextureName[texture]).Bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, points.size() / 3);
    }

    float calDistance(glm::vec3 vec1, glm::vec3 vec2)
    {
        return sqrt((vec1.x - vec2.x) * (vec1.x - vec2.x) + (vec1.y - vec2.y) * (vec1.y - vec2.y) + (vec1.z - vec2.z) * (vec1.z - vec2.z));
    }

    ~Floor(){
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

class Water
{
private:
    /* data */
    std::vector<GLfloat> points;
    std::vector<GLuint> indices;
    int waterWidth = 50;
    int waterLength = 300;
    unsigned int VBO, EBO, VAO;
    glm::vec3 pos;
    struct GerstnerWave
    {
        /* data */
        float s;
        float w;
        float l;
        glm::vec2 D;
    }waves[10];
public:
    Water(){
        pos = glm::vec3(0.0, -0.5, -5.0);
    }
    void init(){
        for(int i = 0;i < waterLength; i++){
            for(int j = 0;j < waterWidth; j++){
                points.push_back((i - waterWidth / 2.0) / 10.0); // x
                points.push_back(0.0);                           // y
                points.push_back((j - waterWidth / 2.0) / 10.0); // z
            }
        }
        /**  (i,j) = width * i + j
         * 
         *      *(i, j)  *(i+1, j)
         *      *(i,j+1) *(i+1,j+1)
        */
        for(int i = 0;i < waterLength - 1; i++){
            for(int j = 0;j < waterWidth - 1; j++){
                indices.push_back(i * waterWidth + j); 
                indices.push_back(i * waterWidth + j + 1);                           
                indices.push_back((i + 1) * waterWidth + j + 1); 
                indices.push_back(i * waterWidth + j); 
                indices.push_back((i + 1) * waterWidth + j);                           
                indices.push_back((i + 1) * waterWidth + j + 1); 
            }
        }
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), points.data(), GL_STATIC_DRAW);
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        srand((unsigned int)time(nullptr));
        int r = rand() % 100, waveCnt = 10;
        glm::vec2 windDir = glm::vec2(cosf(r), sinf(r));
        ResourceManager::GetShader("water").use();
        ResourceManager::GetShader("water").setInt("waveCnt", waveCnt);
        for (int i = 0; i < waveCnt; ++i) {

            waves[i].s = randf(0.1f, 0.2f);
            ResourceManager::GetShader("water").setFloat("wave[" + std::to_string(i) + "].s", waves[i].s);

            // The wave direction is determined by wind direction
            // but have a random angle to the wind direction
            float windAngle = acosf((windDir.x/sqrtf(windDir.x*windDir.x + windDir.y*windDir.y)));
            if (windDir.y < 0) windAngle = -windAngle;
            float waveAngle = randf(windAngle - glm::radians(60.0f),
                                    windAngle + glm::radians(60.0f));
            waves[i].D.x = cos(waveAngle);
            waves[i].D.y = sin(waveAngle);
            ResourceManager::GetShader("water").setVec2("wave[" + std::to_string(i) + "].D", waves[i].D);

            waves[i].w = randf(2.5f, 3.0f);
            ResourceManager::GetShader("water").setFloat("wave[" + std::to_string(i) + "].w", waves[i].w);

            waves[i].l = randf(1.0f, 3.0f);
            ResourceManager::GetShader("water").setFloat("wave[" + std::to_string(i) + "].l", waves[i].l);
        }
    }

    void render(){
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)WINDOW_W / (float)WINDOW_H,
                                                0.1f, 50.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->pos);

        ResourceManager::GetShader("water").use();
        ResourceManager::GetShader("water").setMat4("model", model);
        ResourceManager::GetShader("water").setMat4("view", view);
        ResourceManager::GetShader("water").setMat4("projection", projection);
        ResourceManager::GetShader("water").setFloat("time", static_cast<float>(glfwGetTime()));
        ResourceManager::GetShader("water").setVec3("deepWaterColor", glm::vec3(0.1137f, 0.2745f, 0.4392f));
        ResourceManager::GetShader("water").setVec3("shallowWaterColor", glm::vec3(0.45f, 0.55f, 0.7f));
        ResourceManager::GetShader("water").setVec3("viewPos", camera.Position);
        ResourceManager::GetShader("water").setVec3("lightDir", glm::vec3(10.0f, 1.0f, -12.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT, nullptr);
    }
};

class Map
{
protected:
    std::vector<Floor*> floor;
    Water water;
    std::vector<std::vector<Floor*>> background, verticleFloor;
    GLfloat centerX, centerY, centerZ;

public:
    Map();
    void init();
    void render();
    void update(float delta);
    ~Map();
};

Map::Map(/* args */)
{
    centerX = centerY = centerZ = 0;
    ResourceManager::LoadShader("../shader/mapShader.vs", "../shader/mapShader.fs", nullptr, "map");
    ResourceManager::LoadShader("../shader/waterShader.vs", "../shader/waterShader.fs", nullptr, "water");
    ResourceManager::GetShader("map").use().setVec3("color", glm::vec3(1.0f, 1.0f, 0.2f));
    ResourceManager::LoadTexture("../resources/textures/floor1.png", false, "floor1");
    ResourceManager::LoadTexture("../resources/textures/floor2.png", false, "floor2");
    ResourceManager::LoadTexture("../resources/textures/floor3.png", false, "floor3");
    ResourceManager::LoadTexture("../resources/textures/floor4.png", false, "floor4");
    ResourceManager::LoadTexture("../resources/textures/blue.png", false, "water");
    ResourceManager::LoadTexture("../resources/textures/yellow.png", true, "background1");
    ResourceManager::LoadTexture("../resources/textures/orange.png", true, "background2");
    ResourceManager::LoadTexture("../resources/textures/verti.png", true, "vertical");
}

void Map::init(){
    std::vector<GLfloat> pos;
    for(int i = -FLOOR_NUM;i < FLOOR_NUM;i++){
        pos.push_back(i * 5);
    }

    for(int i = 0;i < FLOOR_NUM * 2;i++){
        floor.push_back(new Floor(rand()%4, glm::vec3(pos[i], 0, 0)));
        floor[i]->init();
    }

    // pos.clear();
    // for(int i = -WATER_NUM;i < WATER_NUM;i++){
    //     pos.push_back(i * 5);
    // }

    // for(int i = 0;i < WATER_NUM * 2;i++){
    //     water.push_back(new Floor(WATER, glm::vec3(pos[i], -0.5, -5.0)));
    //     water[i]->init();
    // }  
    water.init();


    for(int j = 0;j<VERTICAL_NUM;j++){
        std::vector<Floor*> v;
        for (int i = 0; i < WATER_NUM * 2; i++){
            v.push_back(new Floor(VERTICAL, glm::vec3(pos[i], -0.25, -5 * j + 2.5), glm::vec3(1.0f, 0.1f, 1.0f)));
            v[i]->setRotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            v[i]->init();
        }
        verticleFloor.push_back(v);
    }
    for (int i = 0; i < BACKGROUND_NUM_Z; i++){
        std::vector<Floor*> back;
        for (int j = -FLOOR_NUM; j < BACKGROUND_NUM_X; j++){
            int type = (i%2) ? BACKGROUND1 : BACKGROUND2;
            back.push_back(new Floor(type, glm::vec3(j * 5, -0.1 * (i % 2), -(i + 2) * 5)));
        }
        background.push_back(back);
    }

    for(int i = 0;i < background.size();i++){
        for(int j=0;j<background[i].size();j++){
            background[i][j]->init();
        }
    }
}

void Map::update(float delta){
    for(int i = 0;i < floor.size();i++){
        floor[i]->pos.x -= delta;
    }
    if(floor[0]->pos.x < -FLOOR_NUM * 5 - 2.5){
        floor.erase(floor.begin());
        floor.push_back(new Floor(rand()%4, glm::vec3(floor[floor.size() - 1]->pos.x + 5, 0, 0)));
        floor[floor.size() - 1]->init();
    }

    // for(int i = 0;i < water.size();i++){
    //     water[i]->pos.x -= delta;
    // }
    // if (water[0]->pos.x < -WATER_NUM * 5 - 2.5){
    //     water.erase(water.begin());
    //     water.push_back(new Floor(WATER, glm::vec3(water[water.size() - 1]->pos.x + 5, -0.5, -5.0)));
    //     water[water.size() - 1]->init();
    // }

    for(int j = 0;j < verticleFloor.size();j++){
        for(int i = 0;i < verticleFloor[j].size();i++){
            verticleFloor[j][i]->pos.x -= delta;
        }
        if (verticleFloor[j][0]->pos.x < -WATER_NUM * 5 - 2.5){
            verticleFloor[j].erase(verticleFloor[j].begin());
            verticleFloor[j].push_back(new Floor(VERTICAL, 
                glm::vec3(verticleFloor[j][verticleFloor[j].size() - 1]->pos.x + 5, -0.25, -5 * j + 2.5),
                glm::vec3(1.0f, 0.1f, 1.0f)));
            verticleFloor[j][verticleFloor[j].size() - 1]->setRotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            verticleFloor[j][verticleFloor[j].size() - 1]->init();
        }
    }
    for (int i = 0; i < background.size(); i++){
        for (int j = 0; j < background[i].size(); j++){
            background[i][j]->pos.x -= delta;
        }
        if (background[i][0]->pos.x < -FLOOR_NUM * 5 - 2.5){
            background[i].erase(background[i].begin());
            int type = (i % 2) ? BACKGROUND1 : BACKGROUND2;
            background[i].push_back(new Floor(type, 
                glm::vec3(background[i][background[i].size() - 1]->pos.x + 5, -0.1 * (i % 2), -(i + 2) * 5)));
            background[i][background[i].size() - 1]->init();
        }
    }
}

void Map::render(){
    for (auto& x:floor){
        x->render();
    }
    // for (auto &x : water){
    //     x->render();
    // }
    water.render();
    for(int i = 0;i < verticleFloor.size();i++){
        for(int j=0;j<verticleFloor[i].size();j++){
            verticleFloor[i][j]->render();
        }
    }
    for(int i = 0;i < background.size();i++){
        for(int j=0;j<background[i].size();j++){
            background[i][j]->render();
        }
    }
}

Map::~Map()
{
    for (auto& x:floor){
        delete x;
    }
    // for (auto &x : water){
    //     delete x;
    // }
    for(int i = 0;i < verticleFloor.size();i++){
        for(int j=0;j<verticleFloor[i].size();j++){
            delete verticleFloor[i][j];
        }
    }

    for(int i = 0;i < background.size();i++){
        for(int j=0;j<background[i].size();j++){
            delete background[i][j];
        }
    }
}

#endif