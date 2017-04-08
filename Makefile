#! @file
#! @author Vsevolod (Seva) Ivanov
#!

CXX=g++
CXXFLAGS=-std=c++11 -g -Wall -Wextra -Wfatal-errors -pedantic

OPENGL_LINUX=-lGLEW -lGL -lX11 -lpthread -lXrandr -lXi
OPENGL_MAC=-lGLEW -lglfw3 -framework Cocoa -framework OpenGL \
		   -framework IOKit -framework CoreVideo

GLFW_ARCH=-lglfw
GLFW_UNIX=-lglfw3

EXTRA_LIBS=-lSOIL

INCLUDES=-I./src
CXX_FILES=src/Main.cpp src/Window.cpp src/Shader.cpp src/Camera.cpp \
		  src/World.cpp src/Terrain.cpp src/TerrainHeight.cpp

BIN=-o build/world.out

all:
	mkdir -p build

arch: all
	${CXX} ${CXXFLAGS} ${OPENGL_LINUX} ${GLFW_ARCH} ${EXTRA_LIBS} \
		${INCLUDES} ${CXX_FILES} ${BIN}

linux: all
	${CXX} ${CXXFLAGS} ${OPENGL_LINUX} ${GLFW_UNIX} \
		${INCLUDES} ${CXX_FILES} ${BIN}

mac: all
	${CXX} ${CXXFLAGS} ${OPENGL_MAC} ${GLFW_UNIX} \
		${INCLUDES} ${CXX_FILES} ${BIN}

clean:
	rm -rf build/
