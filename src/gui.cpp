#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "constants.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

#define max_size 1
#define min_size -1

void set_num_balls(int num) {
    constants::num_balls = num;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback_gui(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

float sigmoid(float n) {
    return 1.0f / (1.0f + exp(-n));
}

GLFWwindow* start()
{
    glfwSetErrorCallback(error_callback_gui);

    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor == NULL) {
        // Handle error or fallback to windowed mode
    }

    // Get the video mode (resolution, etc.) of the primary monitor
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    if (mode == NULL) {
        // Handle error
    }

    // Create a fullscreen window using the monitor's resolution
    GLFWwindow* window = glfwCreateWindow(
        mode->width, 
        mode->height, 
        "Simple GUI", 
        primaryMonitor, // Pass the monitor object for fullscreen
        NULL
    );

    if (window == NULL) {
        // Handle window creation failure
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);   // MUST come first

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    if (!window) {
        cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    // --- Initialize GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD\n";
        return nullptr;
    }
    // --- Set up OpenGL state ---
    glViewport(0, 0, constants::width, constants::height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    float my_color[4] = {1,1,1,1};
    bool my_tool_active = true;
    bool show_graph = false;
    int num_balls_gui = 1;
    // --- Main loop ---
    while (!glfwWindowShouldClose(window) && my_tool_active)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        ImGui::DragInt("Number of Balls", &num_balls_gui, 1, 1, 100);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if(ImGui::Checkbox("Graph", &show_graph));
        if(show_graph) {
            float samples[100];
            for (int n = 0; n < 100; n++)
                samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
            ImGui::PlotLines("Samples", samples, 100, 0, nullptr, -1.0f, 1.0f, ImVec2(0, 80));
        }
        // Edit a color stored as 4 floats

        ImGui::ColorEdit4("Color", my_color);
        ImVec4 my_color_vec4 = ImVec4(my_color[0], my_color[1], my_color[2], my_color[3]);

        ImGui::TextColored(
            my_color_vec4,
            "Important Stuff"
        );
        ImGui::PushStyleColor(ImGuiCol_Button, my_color_vec4);
        if(ImGui::Button("Start")) {
            set_num_balls(num_balls_gui);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            return window;
        }
        ImGui::PopStyleColor(1);

        // Display contents in a scrolling region
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    set_num_balls(num_balls_gui);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return window;
}
