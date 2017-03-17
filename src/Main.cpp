/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include <stdio.h>

#include "World.hpp"

int main()
{
    const uint8_t w = 200;
    const uint8_t h = 200;

    World *world = new World(w, h);

    world->draw();

    delete world;
    return 0;
}
