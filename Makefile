# Compiler
CXX = g++

# Include paths
CXXFLAGS = -IDependencies/glfw/include \
           -IDependencies/glad/include \
           -IDependencies/imgui \
           -IDependencies/imgui/backends

# Libraries
LIBS = -LDependencies/glfw/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32

# Output
TARGET = sim.exe

# Object files
OBJS = gui.o \
       glad.o \
       imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui_demo.o \
       imgui_impl_glfw.o imgui_impl_opengl3.o

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(TARGET)

# Your source
gui.o: src/gui.cpp
	$(CXX) -c src/gui.cpp $(CXXFLAGS)

# Glad
glad.o: Dependencies/glad/src/glad.c
	$(CXX) -c $< -IDependencies/glad/include

# ImGui core
imgui.o: Dependencies/imgui/imgui.cpp
	$(CXX) -c $< $(CXXFLAGS)

imgui_draw.o: Dependencies/imgui/imgui_draw.cpp
	$(CXX) -c $< $(CXXFLAGS)

imgui_tables.o: Dependencies/imgui/imgui_tables.cpp
	$(CXX) -c $< $(CXXFLAGS)

imgui_widgets.o: Dependencies/imgui/imgui_widgets.cpp
	$(CXX) -c $< $(CXXFLAGS)

imgui_demo.o: Dependencies/imgui/imgui_demo.cpp
	$(CXX) -c $< $(CXXFLAGS)

# ImGui backends
imgui_impl_glfw.o: Dependencies/imgui/backends/imgui_impl_glfw.cpp
	$(CXX) -c $< $(CXXFLAGS)

imgui_impl_opengl3.o: Dependencies/imgui/backends/imgui_impl_opengl3.cpp
	$(CXX) -c $< $(CXXFLAGS)

# Clean build artifacts
clean:
	del *.o $(TARGET)
