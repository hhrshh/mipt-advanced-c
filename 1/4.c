#include <stdio.h>
#include <stdint.h>

// Извлечение бит
// На вход программе подается беззнаковое 32-битное целое число N и натуральное число K (1 ≤ K ≤ 31).
// Требуется взять K младших битов числа N и вывести полученное таким образом число.

// Examples
// Input
// 2 1
// Output
// 0
// Input
// 12 3
// Output
// 4
// Input
// 3 5
// Output
// 3
// Input
// 1024 3
// Output
// 0



int main(void) 
{
    int k;
    uint32_t n;
    scanf("%d %d", &n, &k);
    printf("%u\n", (n << (32 - k) >> (32 - k)));
    return 0;
}