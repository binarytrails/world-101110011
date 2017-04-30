# World-101110011

Procedural world using modern OpenGL in C++.

See: [Roadmap](docs/roadmap.md)

## Controls

**Basic:**

    ESC                     exit
    h                       help
    
    i                       zoom in
    o                       zoom out
    scroll                  zoom in / out
    
    w                       move forward
    s                       move backward
    a                       move left
    d                       move right

    m                       toggle mute
    
    mouse + left click      look around
    
**Extra:**

    t           display triangles
    l           display lines
    p           display points

## Setup

**OSX**

    make mac

**Arch Linux**

Dependencies: ```glew glm glfw-x11 mesa-demos soil```

    make arch

**GNU / Linux**

    make linux

## Libraries

- SOIL : loads texture image
- SFML : sound engine
- FreeType : loading fonts

## Assets

- Textures

    - Terrain
    
        https://opengameart.org/content/terrain-textures-pack-from-stunt-rally-23

    - Skybox

        http://www.custommapmakers.org/skyboxes.php

- Sound
    
    - Background

        ```assets/sound/amb-forest.ogg```

        The size was reduced using Audacity software.

        http://www.freesound.org/people/bajko/sounds/385280

- Code
    
    A vital ressource to the project and learning to implement
    OpenGL concepts was [LearnOpenGL](https://learnopengl.com). It also helped with the implementation of our Shader, Skybox and GUI classes. 

## Documentation

Look into ```docs/``` folder; generate code documentation using ```doxygen``` command at project root.

## Coding Style

- Identation:

    It's a standard to use spaces over tabs. For readability, we recommend 4 spaces which is a widely used standard.

## Authors

| Name                   |  Email                       |
|:----------------------:|:----------------------------:|
| Tarik Abou-Saddik      | tarik.abousaddik@gmail.com   |
| Vsevolod (Seva) Ivanov | seva@tumahn.net              |
| Eric                   | ericchiassonmorgan@gmail.com |
| Sami					 | Sami@boubaker.me			    |
| Justin                 | zigman32@gmail.com           |
