#! @file
#! @author Vsevolod (Seva) Ivanov
#!

CXX=g++
CXXFLAGS=-std=c++11 -g -Wall -Wextra -Wfatal-errors -pedantic \
		-I./src
CXXFLAGS_ARCH = -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
CXXFLAGS_LINUX = -lGLEW -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi

all:
	mkdir -p build

arch: all
	${CXX} ${CXXFLAGS} ${CXXFLAGS_ARCH} src/Main.cpp -o build/world.out

linux: all
	${CXX} ${CXXFLAGS} ${CXXFLAGS_LINUX} src/Main.cpp -o build/world.out

clean:
	rm -rf build/
