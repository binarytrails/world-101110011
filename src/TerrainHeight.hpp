/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class TerrainHeight
{
    public:
        TerrainHeight();

        float get(const float x, const float z);

    private:
        float randFloat(const unsigned int upperBound);

        int seed;
};
