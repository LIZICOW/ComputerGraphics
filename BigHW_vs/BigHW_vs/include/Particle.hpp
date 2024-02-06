#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "manResource.hpp"
#include "model.hpp"
glm::vec3 posStart = glm::vec3(-0.2, 0.0, 0.1);

// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;
    GLfloat lifeCost;

    Particle() : Position(posStart), Color(1.0f), Life(100) {
        GLfloat r = (175 - (rand() % 30)) / 180.0 * 3.14;
        GLfloat t = (60 + (rand() % 60)) / 180.0 * 3.14;
        GLfloat v = (rand() % 100) / 100.0 * 0.04;
        lifeCost = (rand() % 100) / 100.0 + 1.9;
        Velocity = glm::vec3(v * cos(r), v * sin(r), v * cos(t));
    }
    void init() {
        Position = posStart;
        lifeCost = (rand() % 100) / 100.0 + 1.9;
        Life = 100;
    }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(std::string, GLuint amount);
    // Update all particles
    void Update(GLfloat dt, bool);
    // Render all particles
    void Draw();
private:
    // State
    std::vector<Particle> particles;
    GLuint amount;
    // Render state
    std::string name;
    GLuint VAO;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    GLuint firstUnusedParticle();
    // Respawns particle
    void respawnParticle(Particle& particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};

ParticleGenerator::ParticleGenerator(std::string name, GLuint amount)
    : name(name), amount(amount)
{
    this->init();
}
GLuint lastUsedParticle = 0;
void ParticleGenerator::Update(GLfloat dt, bool jump)
{
    // Add new particles 
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        if (p.Life <= 0 && !jump) {
            p.init();
        }
    }
    // Update all particles
    //std::cout << "-----------------------------------------------------------------\n";
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt * p.lifeCost; // reduce life
        if (p.Life > 0.0f)
        {	
            p.Position += p.Velocity * dt;
            p.Color.a -= dt * 0.1;
        }
    }
//    std::cout << "-----------------------------------------------------------------\n";

}

// Render all particles
void ParticleGenerator::Draw()
{
    // Use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    ResourceManager::GetShader(name).use();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
        (float)WINDOW_W / (float)WINDOW_H,
        0.1f, CAMERA_FAR);
    glm::mat4 model = glm::mat4(1.0f);
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            //std::cout << name << "ok\n";
            ResourceManager::GetShader(name).setMat4("view", view);
            ResourceManager::GetShader(name).setMat4("projection", projection);
            ResourceManager::GetShader(name).setMat4("model", model);
            ResourceManager::GetShader(name).setVec3("offset", particle.Position);
            ResourceManager::GetShader(name).setVec4("color", particle.Color);
            ResourceManager::GetShader(name).setFloat("scale", particle.Life);
            ResourceManager::GetTexture(name).Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
    }
    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // Set up mesh and attribute properties
    GLuint VBO;
    GLfloat particle_quad[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);

    // Create this->amount default particle instances
    for (GLuint i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// Stores the index of the last particle used (for quick access to next dead particle)

GLuint ParticleGenerator::firstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, glm::vec3 offset)
{
    GLfloat r = (180 - (rand() % 45)) / 180 * 3.14;
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = glm::vec3(0.0, 0.0, 0.0);
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 10.0f;
    particle.Velocity = glm::vec3(-0.01 * cos(r), 0.01 * sin(r), 0.0);
    //std::cout << particle.Position.x << ' ' << particle.Position.y << ' ' << particle.Position.z << ' ' << '\n';
}

#endif