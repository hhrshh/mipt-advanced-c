#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вид сверху
// В программе описана структура для хранения двоичного дерева:
// typedef struct tree {
// datatype key;
// struct tree *left, *right;
// } tree;
// Необходимо реализовать функцию, которая печатает вид дерева сверху. Функция должна строго соответствовать прототипу:

// void btUpView(tree *root)
// Examples
// Input
// 10 5 15 3 7 13 18 1 6 14 0
// Output
// 0 1 3 5 10 15 18

typedef int datatype;

typedef struct tree {
    datatype key;
    struct tree *left, *right;
    } tree;

tree* createNode(datatype key)
{
    tree* root = (tree*)malloc(sizeof(tree));
    root->key = key;
    root->left = NULL;
    root->right = NULL;
    return root;
}

tree* insert(tree* root, datatype key)
{
    if(root == NULL)
        return createNode(key);
    if(root->key < key)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);

    return root;
}

void deleteTree(tree* root)
{
    if(root == NULL)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    //printf("Free node with key - %d\n", root->key);
    free(root);
}



void btUpView(tree *root)
{
    if(root == NULL)
        return;

    int left[1000] = {0}, i = 1;
    tree* rootCopy = root;

    while(root != NULL)
    {
        left[i++] = root->key;
        root = root->left;
    }
    
    if(i > 1)
    {
        printf("%d", left[--i]);
        --i;
    }

    while(i)
        printf(" %d", left[i--]);   

    rootCopy = rootCopy->right;
    while(rootCopy != NULL)
    {
        printf(" %d", rootCopy->key);
        rootCopy = rootCopy->right;
    }

    printf("\n");
}

int main(void)
{
    int values[] = {10, 5, 15, 3, 7, 13, 18, 1, 6, 14, 0};
    int n = sizeof(values) / sizeof(values[0]);

    tree* root = NULL;

    for(int i = 0; i < n; ++i)
    {
        root = insert(root, values[i]);
        //printf("Added node with key = %d\n", values[i]);
    }
    btUpView(root);
    printf("\n");
    deleteTree(root);
    return 0;
}