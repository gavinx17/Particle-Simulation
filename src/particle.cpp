#include "particle.h"
#include "block.h"
#include "constants.h"
#include <cmath>
#include <vector>

    Particle::Particle(float x, float y, float r) {
        this->x = x;
        this->y = y;
        this->radius = r;
        this->vx = 0.0f;
        this->vy = 0.0f;
        this->velocity = 5.0f; // Default speed, can be adjusted as needed
    }

    void Particle::Update(float dt, std::vector<Block>& blocks, Particle p[], int balls, GLFWwindow* window) {
    for(int i = 0; i < balls; i++)
    {
        // Constants
        const float gravity = -0.9f;       // units per second²
        const float jumpVelocity = 1.5f;   // units per second
        const float floorY = -1.0f;

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            p[i].vx += 2.0f * dt; // velocity scaled by dt
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            p[i].vx -= 2.0f * dt;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            p[i].vy += jumpVelocity * dt; // Set jump velocity (scaled by dt)

        p[i].vy += gravity * dt;

        p[i].x += p[i].vx * dt;
        p[i].y += p[i].vy * dt;

        // --- Floor collision ---
        if (p[i].y - p[i].radius < floorY) {
            p[i].y = floorY + p[i].radius;
            p[i].vy *= -0.8f; // bounce with damping
        }

        // --- Wall collision (optional) ---
        if (p[i].x - p[i].radius <= -1.0f) {
            p[i].x = -1.0f + p[i].radius;
            p[i].vx *= -0.5f; // bounce with damping
        }
        if (p[i].x + p[i].radius >= 1.0f) {
            p[i].x = 1.0f - p[i].radius;
            p[i].vx *= -0.5f;
        }
    }

    // --- Block collisions ---
    for(Block& block: blocks) {
        block.DrawBlock();
        for(int i = 0; i < balls; i++) {
            if (p[i].CheckCollision(p[i], block)) {
                p[i].vx *= -0.5f; // bounce with damping
                p[i].vy *= -0.5f; // bounce with damping
            }
        }
    }
    for(int i = 0; i < balls; i++) {
        for(int j = i + 1; j < balls; j++) {
            if (p[i].CheckCollision(p[i], p[j])) {
                // Simple elastic collision response
                float tempVx = p[i].vx;
                float tempVy = p[i].vy;
                p[i].vx = p[j].vx;
                p[i].vy = p[j].vy;
                p[j].vx = tempVx;
                p[j].vy = tempVy;
            }
        }
    }
}

    void Particle::DrawParticle()  {
        glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * constants::M_PI; i += constants::M_PI / 24)  {
            glVertex3f((cos(i) * radius) + x,(sin(i) * radius) + y, 0.0);
        }
        glEnd();
    }
    bool Particle::CheckCollision(Particle& one, Particle& two) {
        float dx = one.x - two.x;
        float dy = one.y - two.y;
        float distance = sqrt(dx * dx + dy * dy);   // Distance formula
        return distance <= (one.radius + two.radius);
    }
    bool Particle::CheckCollision(Particle& p, Block& b) {
        float halfWidth = b.width / 2.0f;
        float halfHeight = b.height / 2.0f;

        float left = b.x - halfWidth;
        float right = b.x + halfWidth;
        float top = b.y + halfHeight;
        float bottom = b.y - halfHeight;

        return(p.x >= left - p.radius &&
            p.x <= right + p.radius &&
            p.y <= top + p.radius &&
            p.y >= bottom + p.radius);
    }