/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include <stdio.h>
#include <sstream>

#include "World.hpp"

int main(int argc, char *argv[])
{
    uint16_t w = 50;
    uint16_t h = 50;

    printf("Generating a world %ix%i..\n", w, h);
    World *world = new World(w, h, "World-101110011");

    world->draw();

    delete world;
    return 0;
}
