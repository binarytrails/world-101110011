/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "TerrainHeight.hpp"

TerrainHeight::TerrainHeight()
{
    this->seed = rand() % (unsigned int) std::pow(10, 10);
}

float TerrainHeight::get(const float x, const float z)
{
    // set random seed
    srand(x * 6786 + z * 23054 + this->seed);
    return this->randFloat(1.0f) * 2 - 1;
}

// [0, upperBound]
float TerrainHeight::randFloat(const unsigned int upperBound)
{
    return static_cast <float> (
        rand() / (static_cast <float> (RAND_MAX / upperBound))
    );
}
