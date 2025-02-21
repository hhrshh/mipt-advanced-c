#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Submit a solution for D2-Найти брата
// Time limit:	1 s
// Real time limit:	5 s
// Memory limit:	64M
// Найти брата
// В программе описано двоичное дерево:
// typedef struct tree {
// datatype key;
// struct tree *left, *right;
// struct tree *parent; //ссылка на родителя
// } tree;
// Требуется реализовать функцию, которая по ключу возвращает адрес соседнего элемента - брата.
// Если такого ключа нет или у узла нет брата, то необходимо вернуть 0.
// Прототип функции:

// tree * findBrother(tree *root, int key)
// Examples
// Input
// 10 5 15 3 7 13 18 1 6 14 0 3
// Output
// 7

typedef int datatype;

typedef struct tree {
    datatype key;
    struct tree *left, *right;
    struct tree *parent;            //ссылка на родителя
    } tree;

tree* createNode(datatype key)
{
    tree* root = (tree*)malloc(sizeof(tree));
    root->key = key;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
    return root;
}

tree* insert(tree* root, datatype key)
{
    if(root == NULL)
        return createNode(key);

    if(key < root->key)
    {
        tree* leftChild = insert(root->left, key); 
        root->left = leftChild;
        if(leftChild != NULL)
            leftChild->parent = root;
    }
    else
    {
        tree* rightChild = insert(root->right, key); 
        root->right = rightChild;
        if(rightChild != NULL)
            rightChild->parent = root;
    }

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

tree* findBrother(tree* root, int key)
{
    tree *find = root;
    while(find)
    {
        if(find->key == key)
        {
            if(find->parent == NULL)
                return 0;
            if((find->parent->right != NULL) && find->parent->right->key != key)
                return find->parent->right;
            else if((find->parent->left != NULL) && find->parent->left->key != key)
                return find->parent->left;
            else
                return 0;
        }
        if(key < find->key)
            find = find->left;
        else
            find = find->right;
    }
    return 0;
}

int main(void)
{
    int values[] = {10, 5, 15, 3, 7, 13, 18, 1, 6, 14, 0, 3};
    int n = sizeof(values) / sizeof(values[0]);

    tree* root = NULL;

    for(int i = 0; i < n; ++i)
    {
        root = insert(root, values[i]);
        //printf("Added node with key = %d\n", values[i]);
    }

    printf("Serch key: %p", findBrother(root, 4));
    deleteTree(root);
    return 0;
}