// file moj_fft.h
#ifndef MOJ_FFT_H
#define MOJ_FFT_H
#include <complex.h>

#define PI 3.14159265358979323846


void recur_fft(double complex *x, int N);
void recur_ifft(double complex *x, int N);
void iter_fft(double complex *x, int N);
void iter_ifft(double complex *x, int N);
double complex *padding(double complex *x, int *N);

#endif
