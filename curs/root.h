#ifndef ROOT_H
#define ROOT_H

typedef float(*function)(float);

float rootFindTangent(float, float, function, function, function, function, int); // Метод касательных (Ньютона)
float test(float);                                                            // тест корней


#endif