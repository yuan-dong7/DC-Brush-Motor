//
// Created by 鸿鹄 on 2021-01-20.
//

#ifndef PROGRAM_DEVICE_USER_MATH_H_
#define PROGRAM_DEVICE_USER_MATH_H_

#include "math.h"

#define     pi     ((double)3.1415926)
#define     my_pi     ((float)3.1415926)

typedef struct complex_figure {//复数
    double x, y; // real and imaginary parts
} complex;

float user_max(float a[], int n);

complex user_ComplexMul(complex a, complex b);

complex user_ComplexDiv(complex a, complex b);

double mypow(double num, double n);

double MyFmod(double _X, double _Y);

double XSin(double x);

double XCos(double x);

double XTan(double a);

#endif //PROGRAM_DEVICE_USER_MATH_H_
