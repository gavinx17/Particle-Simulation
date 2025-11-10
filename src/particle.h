#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

class Block;

class Particle  {
    public:
        bool growing;
        float x, y;       // current position
        float vx, vy;     // velocity
        float radius;
        float gravity = -.980665f;
        float force = .98f;
        Particle() : x(0), y(0), vx(0.0f), vy(0.0f), radius(0) {}
        Particle(float startX, float startY, float r)
            : x(startX), y(startY), vx(0.0f), vy(0.0f), radius(r) {}

        void DrawParticle();
        void Update(float dt);
        bool CheckCollision(Particle one, Particle two);
        bool CheckCollision(Particle p, Block b);
};
#endif