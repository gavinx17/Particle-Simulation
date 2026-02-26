#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include "block.h"

class Block; // Forward declaration

class Particle {
public:
    float x, y, vx, vy, radius, velocity;

    Particle() = default;
    Particle(float x, float y, float r);

    void Update(float dt, std::vector<Block>& blocks, Particle p[], int balls, GLFWwindow* window);
    void DrawParticle();

    bool CheckCollision(Particle& one, Particle& two);
    bool CheckCollision(Particle& p, Block& b);
};

#endif