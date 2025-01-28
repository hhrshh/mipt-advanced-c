
#include <stdio.h>
#include <stdint.h>

// Распаковать порядок
// Необходимо реализовать программу, которой на входе передаётся вещественное число в типе float,
// она возвращает порядок который хранится в поле EXP в виде десятичного целого числа.
// Функция должна строго соответствовать прототипу

// int extractExp(float)
// Examples
// Input
// 1.0
// Output
// 127
// Input
// 2.0
// Output
// 128
// Input
// 3.99999
// Output
// 128
// Input
// 0.001
// Output
// 117


union intFloat {
    float f;
    uint32_t i;
};

int extractExp(float num)
{
    union intFloat value;
    value.f = num;
    int resault = (value.i >> 23) & 0b11111111;

    return resault;
}

int main(void) 
{
    float num;
    scanf("%f", &num);
    printf("%d\n", extractExp(num));

    return 0;
}