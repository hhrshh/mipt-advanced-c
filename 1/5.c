#include <stdio.h>
#include <stdint.h>

// Поиск последовательности бит
// На вход программе подается беззнаковое 32-битное целое число N и натуральное число K (1 ≤ K ≤ 31).
// Требуется взять K подряд идущих битов числа N так, чтобы полученное число было максимальным.
// Программа должна вывести полученное число.

// Examples
// Input
// 2 1
// Output
// 1
// Input
// 12 3
// Output
// 6
// Input
// 3 5
// Output
// 3
// Input
// 1024 3
// Output
// 4



int main(void) 
{
    int k;
    uint32_t n, max = 0;

    scanf("%d %d", &n, &k);

    for (int i = 0; i <= 32 - k; ++i)
    {   
        uint32_t tmp = (n << (32 - k - i)) >> (32 - k);
        if(tmp > max)
            max = tmp;
    }
    printf("%u\n", max);
    return 0;
}