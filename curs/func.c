#include "func.h"
#include "integral.h"


// Функция 0.6 x + 3 
float f1(float x)
{
    return 0.6 * x + 3;
}


float f2(float x)
{
    return (x - 2) * (x - 2) * (x - 2) - 1;
}

float f3(float x)
{
    return 3 / x;
}

float df1(float x) 
{
    return 0.6;
}

float df2(float x) 
{
    return 3 * (x - 2) * (x - 2);
}

float df3(float x) 
{
    return - (3 / (x * x));
}

// Тестовая функция 1: f(x) = x^2 - 4 (корни: x = 2, x = -2)
float test_f1(float x)
{
    return 8 * x * x * x * x + 32 * x * x * x + 40 * x * x + 16 * x + 1;
}

// Производная тестовой функции 1: f'(x) = 2x
float test_df1(float x) 
{
    return 32 * x * x * x + 96 * x * x + 80 * x + 16;
}

// Тестовая функция 2: f(x) = x - 3 (корень: x = 3)
float test_f2(float x) 
{
    return 0;
}

// Производная тестовой функции 2: f'(x) = 1
float test_df2(float x)
{
    return 0;
}

// 
float test_integral(float x)
{
    return 2;
}