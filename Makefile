#! @file
#! @author Vsevolod (Seva) Ivanov
#!

CXX=g++
CXXFLAGS=-std=c++11 -g -Wall -Wextra -Wfatal-errors -pedantic \
		-lGLEW -lGL -lX11 -lpthread -lXrandr -lXi\
		-I./src

GLFW_ARCH=-lglfw
GLFW_LINUX = -lglfw3

# Linker flags for Mac
CXXFLAGS_MAC =  -std=c++11 -g -Wall -Wextra -Wfatal-errors -pedantic \
-lGLEW -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit \
-framework CoreVideo

CXX_FILES=src/World.cpp src/Window.cpp src/Shader.cpp src/Terrain.cpp src/Main.cpp\
	src/Camera.cpp

all:
	mkdir -p build

arch: all
	${CXX} ${CXXFLAGS} ${GLFW_ARCH} ${CXX_FILES}\
		-o build/world.out

linux: all
	${CXX} ${CXXFLAGS} ${GLFW_LINUX} ${CXX_FILES}\
		-o build/world.out

mac: all
	${CXX} -std=c++11 ${CXXFLAGS_MAC} ${CXX_FILES}\
		-o build/world.out

clean:
	rm -rf build/
