#! @file
#! @author Vsevolod (Seva) Ivanov
#!

CXX=g++
CXXFLAGS=-std=c++11 -g -Wall -Wextra -Wfatal-errors -pedantic \
		-lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi\
		-I./src

all:
	mkdir -p build
	${CXX} ${CXXFLAGS} src/Main.cpp -o build/game.out

clean:
	rm -rf build/
