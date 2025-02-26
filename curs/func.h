#ifndef FUNC_H
#define FUNC_H


float f1(float);        // 0.6 * x + 3
float f2(float);        // (x - 2)^3 - 1
float f3(float);        // 3 / x
float df1(float);       // производная от f1
float df2(float);       // производная от f2
float df3(float x);     // производная от f3

// Тестовые функции
float test_f1(float);
float test_df1(float);
float test_f2(float);
float test_df2(float);
float test_integral(float);


#endif