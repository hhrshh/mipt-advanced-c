#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "func.h"
#include "integral.h"



float calcIntegralSimpson(float xl, float xr, size_t n, function f) 
{
    if (xl >= xr)
    {
        printf("Incorrect integration limits.\n");
        return NAN;
    }
    float sum = 0;
    float h = (xr - xl) / n;
    for(float x = xl + h; x < xr - h; x += h)
    {
        sum += h / 6.0 * (f(x) + 4.0 * f(0.5 * (x + x + h)) + f(x + h));
    }
    return sum;
}


