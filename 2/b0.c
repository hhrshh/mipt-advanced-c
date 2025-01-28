
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// B0-Максимальный блок

// Ограничение времени:	1 с
// Ограничение реального времени:	5 с
// Ограничение памяти:	64M
// Максимальный блок
// Описана структура данных


// typedef struct list {
// uint64_t address;
// size_t size;
// char comment[64];
// struct list *next;
// } list;


// Требуется реализовать только одну функцию, которая в данном списке находит адрес блока памяти занимающий больше всего места.
// Адрес хранится в поле address, поле size - соответствующий размер данного блока. Если список пустой, то функция должна возвращать NULL.
// Если есть несколько таких блоков, то вернуть адрес любого из них.
// Прототип функции:

// uint64_t findMaxBlock(list *head)
// Примеры
// Входные данные
// 3
// 	  140525067852320 10
// 	  140525067852350 30
// 	  140525067852900 100
// Результат работы
// 140525067852900


typedef struct list
{
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;


uint64_t findMaxBlock(list *head)
{
    size_t maxSizeAddress = 0; // Максимальный адрес
    size_t maxSize = 0;
    for(;head; head = head->next)
        if(maxSize < head->size)
        {
            maxSize = head->size;
            maxSizeAddress = head->address;
        }
    return maxSizeAddress; 
}

int main(void)
{

        // Создаем пример списка
    list *head = malloc(sizeof(list));
    head->address = 140525067852320;
    head->size = 10;
    head->next = malloc(sizeof(list));
    head->next->address = 140525067852350;
    head->next->size = 30;
    head->next->next = malloc(sizeof(list));
    head->next->next->address = 140525067852900;
    head->next->next->size = 100;
    head->next->next->next = NULL;
        
        // Ищем элемент с макс полем size
    printf("%lu\n", findMaxBlock(head));

        // Освобождем выделенную память
    free(head->next->next);
    free(head->next);
    free(head);


    return 0;
}


