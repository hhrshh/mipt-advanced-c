#ifndef INTEGRAL_H
#define INTEGRAL_H
#include <stddef.h>

typedef float(*function)(float);

float calcIntegralSquare(float, float, size_t, function); //
float calcIntegralTrap(float, float, size_t, function); //
float calcIntegralSimpson(float, float, size_t, function); //
float calcIntegralMonteCarlo(float, float, float, size_t, function); //

#endif