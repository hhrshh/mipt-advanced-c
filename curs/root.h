#ifndef ROOT_H
#define ROOT_H
//
typedef float(*function)(float);

float rootFindLineSearch(float, float, float, function); //
int signF(float); //
float df(float); //
float ddf(float);
float rootFindLineSearch(float, float, float, function);
float rootFindDiv(float, float, float, function);
float rootFindDiv2(float, float, float, function);
float rootFindChord(float, float, float, function);
float rootFindTangent(float, float, function, function);
float rootFindCombine(float, float, float, function, function, function);

#endif