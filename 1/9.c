
#include <stdio.h>
#include <stdint.h>

// Упаковать массив
// В программе реализована структура данных:
// struct pack_array {
// uint32_t array; // поле для хранения упакованного массива из 0 и 1
// uint32_t count0 : 8; // счетчик нулей в array
// uint32_t count1 : 8; // счетчик единиц в array
// }
// Необходимо реализовать программу, которая упаковывает переданный ей массив из 32-ух элементов 0 и 1 в указанную структуру данных.
// Функция должна строго соответствовать прототипу:

// void array2struct(int [], struct pack_array *)
// Examples
// Input
// 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
// Output
// 4294901760 16 16
// Input
// 1 1 1 1 1 1 1 1 1 0 1 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1
// Output
// 4289134601 17 15



struct pack_array 
{
    uint32_t array; // поле для хранения упакованного массива из 0 и 1
    uint32_t count0 : 8; // счетчик нулей в array
    uint32_t count1 : 8; // счетчик единиц в array
};

void array2struct(int arr[], struct pack_array *pack)
{
    pack->array = 0;
    pack->count0 = 0;
    pack->count1 = 0;
    for(int i = 0; i < 32; i++)
    {
        pack->array <<= 1;
        if(arr[i] == 1)
        {
            pack->array |= 1;
            pack->count1++;
        }
        else
            pack->count0++;
    }
}

int main(void)
{

    int arr[32] = {0};
    for(int i = 0; i < 32; i++) {
        scanf("%d", &arr[i]);
    }
    

    struct pack_array pack;
    array2struct(arr, &pack);

    printf("%u %u %u", pack.array, pack.count0, pack.count1);
    return 0;
}