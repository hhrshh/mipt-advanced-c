#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "func.h"
#include "integral.h"

float calcIntegralSquare(float xl, float xr, size_t n, function f)
{
    float sum = 0;
    float h = (xr - xl) / n;
    for(size_t i = 0; i < n; i++)
    {
        sum += f(xl);
        xl += h;
    }
    return sum*h;
}

float calcIntegralTrap(float xl, float xr, size_t n, function f)
{
    float sum = 0;
    float h = (xr - xl) / n;
    for(float x = xl + h; x < xr - h; x += h) 
    {
        sum += 0.5 * h * (f(x) + f(x + h));
    }
    return sum;
}

float calcIntegralSimpson(float xl, float xr, size_t n, function f) 
{
    float sum = 0;
    float h = (xr-xl)/n;
    for(float x=xl+h; x<xr-h; x+=h) {
        sum += h/6.0*(f(x) + 4.0*f(0.5*(x+x+h)) + f(x+h));
    }
    return sum;
}

float calcIntegralMonteCarlo(float xl, float xr, float fmax, size_t n, function f) 
{
    size_t in_d = 0;
    float width = fabs(xr-xl), height = fmax;
    for(size_t i=0; i<n; i++) {
        float x = ((float)rand()/(float)RAND_MAX) * width - fabs(xl);
        float y = (float)rand()/(float)RAND_MAX * height;
        if(y<f(x))
            in_d++;
    }
    return fabs( (float)in_d / n * width * height );
}

