# COMP371

Group project with OpenGL in C++.

## Group Members

| Name                   |  Email                     |
|:----------------------:|:-------------------------: |
| Tarik Abou-Saddik      | tarik.abousaddik@gmail.com |
| Vsevolod (Seva) Ivanov | seva@tumahn.net            |
| Eric                   | ericchiassonmorgan@gmail.com|
| Alice                  | alicebarkhouse@gmail.com   |
| Sami					 | Sami@boubaker.me			  |

## Helpful Links

[OpenGL Tutorial Website](https://learnopengl.com)

[OpenGL Tutorial Videos](https://www.youtube.com/watch?v=6c1QYZAEP2M&list=PLRwVmtr-pp06qT6ckboaOhnm9FxmzHpbY)

[C++ Tutorial Website](http://www.learncpp.com)

[Getting Started with GLFW] (http://www.glfw.org/docs/latest/quick.html)

[GLEW](http://glew.sourceforge.net)

[OpenGL Math Library (GLM)](http://glm.g-truc.net/0.9.8/index.html)

[OGRE](http://www.ogre3d.org)

## Setup

### Arch Linux

Install these :

    sudo pacman -S glew glm glfw-x11 mesa-demos

Compile with :

    CXX=g++
    CXXFLAGS=-std=c++11 -g -Wall -Wextra -Wfatal-errors -pedantic \
             -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi

Get setup info :

    glxinfo

Test your setup :

    glxgears

## Linking Static GLEW Library

### Mac OSX

    -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

### Linux (general)

     -lGLEW -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi

## Coding Style

### Identation

It's a standard to use spaces over tabs. For readability, I recommend 4 spaces which is a widely used standard.

http://www.learncpp.com/cpp-tutorial/16-whitespace-and-basic-formatting/
