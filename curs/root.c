#include <stdio.h>
#include <math.h>
#include "integral.h"
#include "func.h"
#include "root.h"
<<<<<<< HEAD

=======
>>>>>>> 334a4c628f97a9fb8db80f69fe484ea10a58e1da
int  signF(float x)
{
    return f(x) == 0 ? 0 : (f(x) < 0 ? -1 : +1);
}

float df(float x) 
{
    return 32 * x * x * x + 96 * x * x + 80 * x + 16;
}

float ddf(float x)
{
    return 96 * x * x + 192 * x + 80;
}


float rootFindLineSearch(float xl, float xr, float eps, function f) 
{
    float x, minx = xl, nextstep;
    nextstep = fabs(xr-xl)/(1/eps); //разбиваем на отрезки интервал
    int stepcount=0; 
    for(x=xl; x<xr; x += nextstep, stepcount++) {
        if( fabs(f(x)) < fabs(f(minx)) )
            minx = x;
    }
    printf("Find Line Search root for %d steps\n",stepcount);
    return minx;
}


float rootFindDiv(float xl, float xr, float eps, function f)
{
    int stepcount=0; //число шагов
    float xm;
    while(fabs(xr-xl)>eps) { //вещественный модуль разницы или floatabs
        stepcount++;
        xm=(xl+xr)/2; // середина отрезка
        if(signF(xl) != signF(xm)) //если знак отличается
            xr=xm;
        else
            xl=xm;
    }
    printf("Find Div Search root for %d steps\n",stepcount); //статистика
    return (xl+xr)/2;
}

 float rootFindDiv2(float xl, float xr, float eps, function f)
 {
    int stepcount=0; //число шагов
    float xm;
    while(fabs(xr-xl)>eps) { //вещественный модуль разницы
        stepcount++;
        xm=(xl+xr)/2; // середина отрезка
        if(f(xr)==0) { // нашли решение на правой границе
            printf("Find root for %d steps\n",stepcount);
            return xr;
        }        
        if(f(xl)==0) { // нашли решение на левой границе
            printf("Find root for %d steps\n",stepcount);
            return xl;
        }
        if(signF(xl) != signF(xm)) //если знак отличается
            xr=xm;
        else
            xl=xm;
    }
    printf("Find root for %d steps\n",stepcount); //статистика
    return (xl+xr)/2;
}

float rootFindChord(float xl, float xr, float eps, function f)
{
    int stepcount=0; 
    while(fabs(xr - xl) > eps) {
        xl = xr - (xr - xl) * f(xr) / (f(xr) - f(xl));
        xr = xl - (xl - xr) * f(xl) / (f(xl) - f(xr));
        stepcount++;
    }
    printf("Find Chord Search root for %d steps\n",stepcount);
    return xr;
}

float rootFindTangent(float xn, float eps, function f, function df ) 
{
    float x1  = xn - f(xn)/df(xn);
    float x0  = xn;
    int stepcount=0;    
    while(fabs(x0-x1) > eps) {
        x0 = x1;
        x1 = x1 - f(x1)/df(x1);
        stepcount++;    
    }
    printf("Find Tangent Search root for %d steps\n",stepcount);   
    return x1;
}

float rootFindCombine(float xl, float xr, float eps, function f, function df, function ddf ) 
{
   int stepcount=0;    
   while(fabs(xl-xr) > 2*eps) {
       if( f(xl)*ddf(xl)<0 )
           xl = xl - (f(xl)*(xl - xr))/(f(xl) - f(xr));
       else
           xl = xl - f(xl)/df(xl);
       if( f(xr)*ddf(xr)<0 )
           xr = xr - (f(xr)*(xr - xl))/(f(xr) - f(xl));
       else
           xr = xr - f(xr)/df(xr);
       stepcount++;        
   } printf("Find Tangent Search root for %d steps\n",stepcount); return (xl+xr)/2;
}