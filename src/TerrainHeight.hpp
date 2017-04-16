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

//! Generates the terrain elevation by using custom HeightMap with noise funcs
class TerrainHeight
{
    public:
        TerrainHeight();

        float get(const float x, const float z);

        void setAmplitude(float a);

    private:
        //! Smooth noise
        float snoise(float x, float z);
        //! Sharp noise
        float noise(const float x, const float z);

        //! Interpolation to get float values
        float inoise(float x, float z);
        float icurve(float a, float c, float b);

        //! Random float
        float randf(const unsigned int upperBound);

        int   SEED;
        int   OCTAVES;
        float AMPLITUDE;
        float IRREGULARITY;
};
