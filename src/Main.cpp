/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include <stdio.h>
#include <sstream>

#include "World.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("You need to provide width & height.\n");
        return 1;
    }

    unsigned int w;
    unsigned int h;
    std::stringstream w_str(argv[1]);
    std::stringstream h_str(argv[2]);
    w_str >> w; h_str >> h;

    printf("Generating a world %ix%i..\n", w, h);
    World *world = new World(w, h);

    world->draw();

    delete world;
    return 0;
}
