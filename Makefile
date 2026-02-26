CXX = g++
CC  = gcc

CXXFLAGS = -std=c++17 \
           -IDependencies/glfw/include \
           -IDependencies/glad/include \
           -IDependencies/imgui \
           -IDependencies/imgui/backends

LIBS = -LDependencies/glfw/lib-mingw-w64 \
       -lglfw3 \
       -lopengl32 \
       -lgdi32 \
       -lwinmm \
       -luser32 \
       -lkernel32 \
       -lshell32


TARGET = sim.exe

SRC = \
	src/main.cpp \
	src/gui.cpp \
	src/constants.cpp \
	src/particle.cpp \
	src/block.cpp \
	src/button.cpp \
	Dependencies/glad/src/glad.c \
	Dependencies/imgui/imgui.cpp \
	Dependencies/imgui/imgui_draw.cpp \
	Dependencies/imgui/imgui_tables.cpp \
	Dependencies/imgui/imgui_widgets.cpp \
	Dependencies/imgui/imgui_demo.cpp \
	Dependencies/imgui/backends/imgui_impl_glfw.cpp \
	Dependencies/imgui/backends/imgui_impl_opengl3.cpp

OBJS = $(SRC:.cpp=.o)
OBJS := $(OBJS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

%.o: %.c
	$(CC) -c $< -IDependencies/glad/include -o $@

clean:
	rm -f src/*.o
	rm -f Dependencies/glad/src/*.o
	rm -f Dependencies/imgui/*.o
	rm -f Dependencies/imgui/backends/*.o
	rm -f sim.exe
