#include "particle.h"
#include "block.h"
#include <cmath>

    void Particle::Update(float dt) {
        const float bottom = -0.99f + radius;
        const float top = 0.99f - radius;
        const float friction = 0.74f;
        const float bounceDamping = 0.74f;
        const float settleThreshold = 0.01f;  // when to start settling
        const float settleDamping = 0.95f;    // how quickly it comes to rest (closer to 1 = slower)

        // Left and right bounds
        if (x - radius <= -1.0f) {
            vx *= -0.6f;
            x = -0.99f + radius;
        }
        if (x + radius >= 1.0f) {
            vx *= -0.6f;
            x = 0.99f - radius;
        }

        // Bottom collision
        if (y - radius <= -1.0f) {
            y = bottom;
            vy *= -bounceDamping;
            vx *= friction; // horizontal slowdown

            // Smoothly settle if the bounce is very small
            if (fabs(vy) < settleThreshold) {
                vy *= settleDamping;
                vx *= settleDamping;

                // If nearly still, stop completely
                if (fabs(vy) < 0.001f && fabs(vx) < 0.001f) {
                    vy = 0.0f;
                    vx = 0.0f;
                }
            }
        }

        // Top collision
        if (y + radius >= 1.0f) {
            vy *= -0.65f;
            y = top;
        }

        // Gravity
        vy += (gravity * dt);

        // Update position
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