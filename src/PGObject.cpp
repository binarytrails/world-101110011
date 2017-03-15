#include "PGObject.h"

float PGObject::randNum(float min, float max) {
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_real_distribution<float> uni(min,max); // guaranteed unbiased

    return uni(rng);
}