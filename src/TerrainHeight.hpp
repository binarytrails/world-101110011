/*
 * @file
 * @author Vsevolod (Seva) Ivanov
 * @brief based on docs/research/PerlinNoise.pdf
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

class TerrainHeight
{
    public:
        TerrainHeight();

        float get(const float x, const float z);

        void setAmplitude(float a);

    private:
        // smooth noise
        float snoise(float x, float z);
        // sharp noise
        float noise(const float x, const float z);

        // interpolation to get float values
        float inoise(float x, float z);
        float icurve(float a, float c, float b);

        // random float
        float randf(const unsigned int upperBound);

        int   SEED;
        int   OCTAVES;
        float AMPLITUDE;
        float IRREGULARITY;
};
