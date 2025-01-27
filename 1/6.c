#include <stdio.h>
#include <stdint.h>

// Подсчет битов
// На вход программе подается беззнаковое 32-битное целое число N.
// Требуется найти количество единичных битов в двоичном представлении данного числа.

// Examples
// Input
// 15
// Output
// 4
// Input
// 16
// Output
// 1
// Input
// 17
// Output
// 2



int main(void) 
{
    int counter;
    uint32_t n;

    for (scanf("%d", &n); counter += n & 1, n; n >>= 1)
    {}

    printf("%u\n", counter);
    return 0;
}