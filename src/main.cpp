#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "imgui.h"
#include "particle.h"
#include "block.h"
#include "gui.h"
#include "constants.h"
#include "button.h"

using namespace std;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
    GLFWwindow* result_window = start();
    int balls = constants::num_balls;
    if (!result_window) {
        cerr << "Failed to initialize GUI\n";
        return -1;
    }
    srand(static_cast<unsigned>(time(0)));
    Particle p[balls];
    std::vector<Block> blocks;

    for(int i = 0; i < balls; i++) {
        float startX = constants::min_size + ((float)rand() / RAND_MAX) * (constants::max_size - constants::min_size);
        float startY = constants::min_size + ((float)rand() / RAND_MAX) * (constants::max_size - constants::min_size);
        p[i] = Particle(startX, startY, 0.03f); // Last argument is radius for every particle.
    }


    GLFWwindow* window = result_window; // Use the window created in start()
    if (!window) {
        cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // --- Initialize GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    blocks.emplace_back();
    blocks[0].InitBlock(0.5f, 0.245f, 0.5f, 0.1f);

    blocks.emplace_back();
    blocks[1].InitBlock(-0.5f, 0.245f, 0.5f, 0.1f);

    // --- Set up OpenGL state ---
    glViewport(0, 0, constants::width, constants::height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // --- Main loop ---
   // --- Main loop ---
    float lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))  {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.078f, 0.75f, 0.078f);

        float currentTime = glfwGetTime();
        float dt = (currentTime - lastFrameTime) / p[0].velocity; // Scale down dt for smoother movement
        lastFrameTime = currentTime;

        // --- Draw particles ---
        for(int i = 0; i < balls; i++)
        {
            p[i].Update(dt, blocks, p, balls, window);
            p[i].DrawParticle();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
glfwDestroyWindow(window);
glfwTerminate();
return 0;
}
