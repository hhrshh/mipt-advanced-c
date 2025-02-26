#ifndef INTEGRAL_H
#define INTEGRAL_H
#include <stddef.h>

typedef float(*function)(float);                           // тип для подстановки функции в функцию
float calcIntegralSimpson(float, float, size_t, function); // Вычисление интеграла по методу Симпсона
float testIntegral(float);


#endif