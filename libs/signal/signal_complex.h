#ifndef SIGNAL_COMPLEX_H
#define SIGNAL_COMPLEX_H

#include <iostream>
#include <cmath>

typedef struct {
    float real;
    float imaginary;
} Complex;

inline Complex cadd(Complex a, Complex b) {
    return {
        a.real + b.real,
        a.imaginary + b.imaginary
    };
}

inline Complex csub(Complex a, Complex b) {
    return {
        a.real - b.real,
        a.imaginary - b.imaginary
    };
}

inline Complex cmul(Complex a, Complex b) {
    return {
        a.real*b.real - a.imaginary*b.imaginary,
        a.real*b.imaginary + b.real*a.imaginary
    };
}

inline Complex cdiv(Complex a, Complex b) {

    float d = (b.real)*(b.real) + (b.imaginary)*(b.imaginary);

    return {
        (a.real*b.real + a.imaginary*b.imaginary)/d,
        (a.imaginary*b.real - a.real*b.imaginary)/d
    };
}

inline Complex cpolar(float angle) {
    return {
        std::cos(angle),
        std::sin(angle)
    };
}

#endif