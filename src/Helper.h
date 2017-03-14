#ifndef HELPER_H
#define HELPER_H

#include <math.h>

class Helper {
public:
    Helper();
    virtual ~Helper();
    static float getSin(float val);
    static float getCos(float val);
};

#endif /* HELPER_H */

