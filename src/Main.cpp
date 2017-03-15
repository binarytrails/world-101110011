/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include <stdio.h>

#include "World.hpp"

int main()
{
    World *world = new World();

    world->draw();

    delete world;
    return 0;
}
