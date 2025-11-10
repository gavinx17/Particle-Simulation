#include "particle.h"
#include "block.h"
#include <cmath>

void Particle::Update(float dt)   {
        if(x - radius <= -1.0f)    {
            vx *= -0.6f;
            x = -.8f + radius;
        }
        if(x + radius >= 1.0f)    {
            vx *= -0.6f;
            x = .8f - radius;
        }
        if (y - radius <= -1.0f) {
            y = -.8f + radius;
            vy *= -0.74f;
            vx *= 0.74f; // horizontal friction
        }
        if(y - radius >= 1.0f)    {
            vy *= -0.65; // slow down as it bounces
            y = .8f - radius;
        }
        vy += (gravity * dt);
        x += (vx * dt);
        y += (vy * dt);
    }
    void Particle::DrawParticle()  {
        glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * M_PI; i += M_PI / 24)  {
            glVertex3f((cos(i) * radius) + x,(sin(i) * radius) + y, 0.0);
        }
        glEnd();
    }
    bool Particle::CheckCollision(Particle one, Particle two) {
        float dx = one.x - two.x;
        float dy = one.y - two.y;
        float distance = sqrt(dx * dx + dy * dy);   // Distance formula
        return distance <= (one.radius + two.radius);
    }
    bool Particle::CheckCollision(Particle p, Block b) {
        // Get the half extents of the block
        float halfWidth = b.width / 2.0f;
        float halfHeight = b.height / 2.0f;

        // Compute AABB edges
        float left = b.x - halfWidth;
        float right = b.x + halfWidth;
        float top = b.y + halfHeight;
        float bottom = b.y - halfHeight;

        // Find the closest point on the block to the circle center
        float closestX = std::max(left, std::min(p.x, right));
        float closestY = std::max(bottom, std::min(p.y, top));

        // Calculate distance between circle center and closest point
        float dx = p.x - closestX;
        float dy = p.y - closestY;

        // If distance < radius, collision
        return (dx * dx + dy * dy) < (p.radius * p.radius);
    }