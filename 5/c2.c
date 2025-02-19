#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int datatype;

void push(datatype v); // используется для вычислений

datatype pop(void);

int isEmpty(void); // определяет пустой ли стек st
void operate(char c); // вычисляем два верхних значения на стеке st

#define MAX_STACK_SIZE 255

datatype st[MAX_STACK_SIZE]; // массив - стек
int pst=0; // заполненность  стека

void push(datatype v) // используется для вычислений
{
    st[pst++]=v;
} 
datatype pop()
{
    if(pst<=0) {
        fprintf(stderr, "Error. Stack underflow");
        return 1;
    } else if(pst>MAX_STACK_SIZE) {
        fprintf(stderr, "Error. Stack overflow");
        return 1;
    }
    return st[--pst];
}

 int isEmpty() // определяет пустой ли стек st
{
    return (pst<=0);
}

void operate(char c) // вычисляем два верхних значения на стеке st
{
    datatype arg1=pop(),arg2=pop();
    switch(c)
    {
        case '+':
            push(arg1+arg2);
        break;
        case '-':
            push(arg2-arg1);
        break;      
        case '*':
            push(arg1*arg2);
        break;              
        case '/':
            push(arg2/arg1);
        break;              
    }
}
_Bool isDigit(char c) // проверяем является ли символ цифрой
{
    return ((c>='0')&&(c<='9'));
}   

_Bool isOperator(char c) {
    return c=='+' ||  c=='-' ||  c=='*' || c=='/';
}

 // main.c
//#include <stdio.h>
//#include "stack.h"

int main(void)
{
    char c;
    char str[1000];    
    int len=0;// 1 2 + 4 * 3 + // 3 5 + 10 * 17 2 * -
              //answer = 15    // answer = 46
    //~ scanf("%[^\n]",str);
    //~ len=strlen(str);
    while((c=getchar())!='\n')
        str[len++]=c;
    str[len]=0; 
    //~ printf("str=%slen=%d\n",str,len);
    for(int i=0;i<len;i++)    
    {
        if(isDigit(str[i]))
        {
            datatype number;//strtof( const char * string, char ** endptr, int basis );
            for(number=0;isDigit(str[i]);i++)
                number=number*10+str[i]-'0';
            //~ printf("%d \n",number);
            push(number);
        }        
        else
        {
            if(i>=len)            
                break;
            if(isOperator(str[i]))
                 operate(str[i]);
        }
    }
    printf("%d\n",pop());    
    return 0;
}