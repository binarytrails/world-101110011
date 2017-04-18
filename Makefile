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

EXTRA_LIBS=-lSOIL -lsfml-audio -lfreetype 

INCLUDES=-I./src
INCLUDES_MAC = -I/usr/local/include/freetype2 -I./src
CXX_FILES=src/Main.cpp src/Window.cpp src/Shader.cpp src/Camera.cpp \
		  src/World.cpp src/Terrain.cpp src/TerrainHeight.cpp \
		  src/Skybox.cpp \
		  src/GLObject.cpp src/Helper.cpp src/lNode.cpp src/PGObject.cpp \
		  src/Forest.cpp  src/Point.cpp src/PGTree*.cpp \
		  src/Vector.cpp src/RotationalObject.cpp src/GUI.cpp

BIN=-o build/world.out

all:
	mkdir -p build

arch: all
	${CXX} ${CXXFLAGS} ${OPENGL_LINUX} ${GLFW_ARCH} \
		${EXTRA_LIBS} ${SOUND_LINUX} \
		${INCLUDES} ${CXX_FILES} ${BIN}

linux: all
	${CXX} ${CXXFLAGS} ${OPENGL_LINUX} ${GLFW_UNIX} ${EXTRA_LIBS} \
		${INCLUDES} ${CXX_FILES} ${BIN}

mac: all
	${CXX} ${CXXFLAGS} ${OPENGL_MAC} ${GLFW_UNIX} ${EXTRA_LIBS} \
		${INCLUDES_MAC} ${CXX_FILES} ${BIN}

# Change code to use a different PGTree to compile a different one
trees-arch: all
	${CXX} ${CXXFLAGS} ${OPENGL_LINUX} ${GLFW_ARCH} ${EXTRA_LIBS} \
		${INCLUDES} -o build/trees.out \
		src/GLObject.cpp src/Helper.cpp src/lNode.cpp src/PGObject.cpp \
		src/MainTrees.cpp src/Shader.h src/Point.cpp src/PGTree*.cpp \
		src/Vector.cpp src/RotationalObject.cpp

skybox-arch: all
	${CXX} ${CXXFLAGS} ${OPENGL_LINUX} ${GLFW_ARCH} ${EXTRA_LIBS} \
		${INCLUDES} -o build/skybox.out \
		src/Skybox.cpp src/Camera.cpp src/Window.cpp src/Shader.cpp

skybox-mac: all
	${CXX} ${CXXFLAGS} ${OPENGL_MAC} ${GLFW_UNIX} ${EXTRA_LIBS} \
		${INCLUDES} -o build/skybox.out \
		src/Skybox.cpp src/Camera.cpp src/Window.cpp src/Shader.cpp

weather-arch: all
	${CXX} ${CXXFLAGS} ${OPENGL_LINUX} ${GLFW_ARCH} ${EXTRA_LIBS} \
		${INCLUDES} -o build/weather.out \
		src/MainCloud.cpp src/Cloud.cpp src/Particle.cpp src/Wind.cpp

clean:
	rm -rf build/
