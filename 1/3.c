#include <stdio.h>
#include <stdint.h>

// Циклический сдвиг
// На вход программе подается беззнаковое 32-битное целое число N и натуральное число K (1 ≤ K ≤ 31).
// Требуется циклически сдвинуть битычисла N вправо на K битов и вывести полученное таким образом число.

// Examples
// Input
// 2 1
// Output
// 1
// Input
// 1 2
// Output
// 1073741824
// Input
// 3 1
// Output
// 2147483649
// Input
// 12 2
// Output
// 3
// Input
// 19 3
// Output
// 1610612738



int main(void)  
{
    int k;
    uint32_t n;
    scanf("%d %d", &n, &k);
    printf("%u\n", (n >> k) | (n << (32 - k)));
    return 0;
}