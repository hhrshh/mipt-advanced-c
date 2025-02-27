#ifndef ROOT_H
#define ROOT_H


typedef float(*function)(float);

// Прототип функции для метода касательных
float rootFindTangent(float, float, function, function, function, function, int); // Метод касательных (Ньютона)


#endif