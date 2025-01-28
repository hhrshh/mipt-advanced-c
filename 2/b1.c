
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// B1-Всего памяти
// Ограничение времени:	1 с
// Ограничение реального времени:	5 с
// Ограничение памяти:	64M
// Всего памяти

// Описана структура данных для хранения информации об использованной памяти:
// typedef struct list {
// void *address;
// size_t size;
// char comment[64];
// struct list *next;
// } list;

// Требуется реализовать только одну функцию, которая анализирует данный список и возвращает сколько всего памяти используется. Адрес хранится в поле address, поле size - соответствующий размер данного блока. Если список пустой, то функция должна возвращать NULL.
// Прототип функции:

// size_t totalMemoryUsage(list *head)
// Примеры
// Входные данные
// 3
// 	  140525067852320 10
// 	  140525067852350 30
// 	  140525067852900 100
// Результат работы
// 140


typedef struct list
{
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;


uint64_t totalMemoryUsage(list *head)
{
    size_t totalMemoryUsage = 0; // Сумма памяти
    for(;head; head = head->next)
        totalMemoryUsage += head->size;
    return totalMemoryUsage;
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
        
        // Если вывод не NULL выводим общее кол-во память
    printf("%lu\n", totalMemoryUsage(head));

        // Освобождем выделенную память
    free(head->next->next);
    free(head->next);
    free(head);


    return 0;
}


