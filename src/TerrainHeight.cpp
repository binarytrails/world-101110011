/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "TerrainHeight.hpp"

TerrainHeight::TerrainHeight():
    AMPLITUDE(3),
    SEED(rand() % (unsigned int) std::pow(10, 10))
{
}

/* TODO
 *      assert positive numbers
 */
float TerrainHeight::get(const float x, const float z)
{
    // octaves & sharpness
    float t =  inoise (x/4, z/4) * AMPLITUDE;
          t += inoise (x/2, x/2) * AMPLITUDE/3;
          t += inoise (x,   z)   * AMPLITUDE/9;

    /* TODO more controls
    float d = (float) pow(2, o-1);

    for (int i=0; i < o; i++)
        float f = (float) (pow(2, i) / d);
        float a = (float) pow(r, i) * AMPLITUDE;
        t += inoise(x*f, z*f) * a;
    */
    return t;
}

// Private

// smooth noise
float TerrainHeight::snoise(float x, float z)
{
    // dividor reduces their influence
    // corners
    float c = noise(x-1, z+1) + noise(x+1, z+1) +
              noise(x-1, z-1) + noise(x+1, z-1);
    c /= 16;
    // sides
    float s = noise(x-1, z) + (x, z-1) +
              noise(x+1, z) + (x, z+1);
    s /= 8;
    // middle
    float m = noise(x, z);
    m /= 4;
    // combined influence
    return c + s + m;
}

// sharp noise
float TerrainHeight::noise(const float x, const float z)
{
    // set random seed
    srand(x * 6786 + z * 23054 + this->SEED);
    return this->randf(1.0f) * 2 - 1;
}

float TerrainHeight::inoise(float x, float z)
{
    // noise input
    int ix = (int) x; float fx = x - ix;
    int iz = (int) z; float fz = z - iz;
    // smooth noise
    float n  = snoise   (ix,   iz);
    float n2 = snoise   (ix+1, iz);
    float n3 = snoise   (ix,   iz+1);
    float n4 = snoise   (ix+1, iz+1);
    // interpolated curves
    float i1 = icurve  (n,    n2,  fx);
    float i2 = icurve  (n3,   n4,  fx);
    return     icurve  (i1,   i2,  fz);
}

// interpolated curve
float TerrainHeight::icurve(float a, float b, float bl)
{
    double angle = bl * M_PI;
    float f = (float)(1.0f - cos(angle)) * 0.5f;
    return a * (1.0f - f) + b * f;
}

// [0, upperBound]
float TerrainHeight::randf(const unsigned int upperBound)
{
    return static_cast <float> (
        rand() / (static_cast <float> (RAND_MAX / upperBound))
    );
}
