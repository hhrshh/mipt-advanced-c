#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Префикс и суффикс
// На стандартном потоке ввода задаются две символьные строки, разделённые символом перевода строки.
// Каждая из строк не превышает подлине 104. В строках не встречаются пробельные символы.
// На стандартный поток вывода напечатайте два числа, разделённых пробелом: первое число — длина наибольшего префикса первойстроки,
// являющегося суффиксом второй; второе число — наоборот, длина наибольшего суффикса первой строки,
// являющегося префиксомвторой. Сравнение символов проводите с учётом регистра (т.е. символы 'a' и 'A' различны).

// Examples
// Input
// don't_panic
// nick_is_a_mastodon
// Output
// 3 3
// Input
// monty_python
// python_has_list_comprehensions
// Output
// 0 6
// T0G0gx1HHH94tAAEG1tA8E4Y6OLJVI05aB713hDCCN363sCuteeKY5yh2O1F0As5BH6lfOaJbPjIQAZV87DpyhRbVG4kgXRKWDE7C1EL4EA4dBDC3LHQF9Kj
// 9UD6J6fM4HT0G0gx1HHH94tAAEG1tA8E4Y6OLJVI05aB713hDCCN363sCuteeKY5yh2O1F0As5BH6lfOaJbPjIQAZV87DpyhRbVG

int prefix_suffix(char* str1, char* str2) {
    int str_len1 = strlen(str1);
    int str_len2 = strlen(str2);
    int max_len = 0;

    for (int i = 1; i <= str_len1 && i <= str_len2; i++) {
        if (strncmp(str1, str2 + str_len2 - i, i) == 0) {
            max_len = i;
        }
    }

    return max_len;
}

int suffix_prefix(char* str1, char* str2) {
    int str_len1 = strlen(str1);
    int str_len2 = strlen(str2);
    int max_len = 0;

    for (int i = 1; i <= str_len1 && i <= str_len2; i++) {
        if (strncmp(str1 + str_len1 - i, str2, i) == 0) {
            max_len = i;
        }
    }

    return max_len;
}

int main(void)
{
    char str1[10001], str2[10001];
    scanf("%s", str1);
    scanf("%s", str2);
    str1[strcspn(str1, "\n")] = 0;
    str2[strcspn(str2, "\n")] = 0;

    printf("%d %d\n", prefix_suffix(str1, str2), suffix_prefix(str1, str2));
    return 0;
}