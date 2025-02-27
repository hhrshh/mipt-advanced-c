#ifndef INTEGRAL_H
#define INTEGRAL_H
#include <stddef.h>

typedef float(*function)(float);// тип для подстановки функции в функцию
// Вычисление интеграла по методу Симпсона
float calcIntegralSimpson(float, float, size_t, function); 


#endif