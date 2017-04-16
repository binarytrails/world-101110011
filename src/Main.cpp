/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include <stdio.h>
#include <sstream>

#include "World.hpp"

int main(int argc, char *argv[])
{
    /* The size in cells of the initial world.
     * Note: keep these values low for fast runtime generation
     */
    uint16_t w = 50; // along x-axis
    uint16_t h = 50; // along z-axis

    printf("Generating a world %ix%i..\n", w, h);
    World *world = new World(w, h, "World-101110011");

    world->draw();

    delete world;
    return 0;
}
