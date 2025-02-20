#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Отсортировать слова
// Дана строка, состоящая из английских букв и пробелов. В конце строки символ точка.
// Все слова разделены одним пробелом. Необходимо составить из слов односвязный список и упорядочить по алфавиту.
// Список необходимо удалить в конце программы. Для сравнение строк можно использовать strcmp.
// Необходимо использовать данную структуры организации списка.

// struct list {
// char word[20];
// struct list *next;
// }

// Необходимо реализовать односвязный список и обслуживающие его функции.
// add_to_list
//  swap_elements
//  print_list
//  delete_list

// Examples
// Input
// efg abcd zzz.
// Output
// abcd efg zzz
// Input
// fffff kkkkkk a.
// Output
// a fffff kkkkkk


// Структура узла односвязного списка
typedef struct list {
    char word[20];
    struct list *next;
} list;

// Функция для добавления слова в список
void add_to_list(list** head, char* word) {
    list* new_node = (list*)malloc(sizeof(list));
    strncpy(new_node->word, word, 20);
    new_node->next = *head; // Указываем на текущую голову
    *head = new_node;       // Обновляем голову списка
}

// Функция для сортировки списка (пузырьковая сортировка)
void sort_list(list** head) {
    if (*head == NULL) return;

    int swapped;
    list* ptr1;
    list* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->word, ptr1->next->word) > 0) {
                // Меняем слова местами
                char temp[20];
                strncpy(temp, ptr1->word, 20);
                strncpy(ptr1->word, ptr1->next->word, 20);
                strncpy(ptr1->next->word, temp, 20);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Функция для вывода списка
void print_list(list* head) {
    if (head == NULL) return; // Если список пуст, ничего не выводим
    head = head->next;
    // Выводим первое слово без пробела перед ним
    printf("%s", head->word);
    head = head->next;

    //Выводим остальные слова с пробелом перед ними
    while (head != NULL) {
        printf(" %s", head->word);
        head = head->next;
    }
    printf("\n"); // Перевод строки после всех слов
}

// Функция для удаления списка
void delete_list(list** head) {
    list* current = *head;
    list* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL; 
}

int main(void)
{
    char str[1000];
    fgets(str, sizeof(str), stdin);
    list* head = NULL;
    const char delimiters[] = " .";

    // Разбиение строки на токены
    char* token = strtok(str, delimiters);
    while (token != NULL) {
        add_to_list(&head, token);
        token = strtok(NULL, delimiters);
    }

    // Сортируем список
    sort_list(&head);

    // Выводим отсортированный список
    print_list(head);
    // Удаляем список
    delete_list(&head);
    return 0;
}