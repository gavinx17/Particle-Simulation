Ongoing development of a gravity simulation with several different masses.

TODO:
Add more Particle Objects
Add Collision between Particle Objects
Move Particle Objects with mouse movements

use this command in root to run: g++ src/main.cpp src/block.cpp src/particle.cpp Dependencies/glad/src/glad.c -I"Dependencies/glfw-3.4/include" -I"Dependencies/glad/include"  -L"Dependencies/glfw-3.4/lib-mingw-w64" -lglfw3 -lopengl32 -lgdi32 -o .\sim.exe
