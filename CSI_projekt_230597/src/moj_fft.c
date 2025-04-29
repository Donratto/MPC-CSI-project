// file moj_fft.c
// https://cp-algorithms.com/algebra/fft.html
// https://lloydrochester.com/post/c/example-fft/
#include <math.h>
#include <stdlib.h>
#include <complex.h> // for complex numbers and operations
#include "moj_fft.h"

// Original size of the input array before padding
int org_N;
double zero_threshold = 1e-10;

// prototypes
void inner_recur_fft(double complex *x, int N, int inverse);
void inner_iter_fft(double complex *x, int N, int inverse);

double complex *padding(double complex *x, int *N) {
    int m = 1;
    while (m < *N) m <<= 1; // Find the next power of 2

    if (m == *N) {
        // No padding needed, return the original array
        return x;
    }

    // Allocate memory for the padded array
    double complex *x_padded = (double complex *)malloc(m * sizeof(double complex));

    // Copy original data and pad with zeros
    for (int i = 0; i < m; i++) {
        if (i < *N) {
            x_padded[i] = x[i];
        } else {
            x_padded[i] = 0.0 + 0.0 * I; // Pad with zeros
        }
    }
    free(x);
    *N = m;
    return x_padded;
}

void recur_fft(double complex *x, int N) {
    x = padding(x, &N);
    inner_recur_fft(x, N, 0);
    for (int i = 0; i < N; i++) {
        // Handle -0.0 case
        if (cabs(x[i]) < zero_threshold) x[i] = 0.0 + 0.0 * I;
    }
}

void recur_ifft(double complex *x, int N) {
    inner_recur_fft(x, N, 1);
    for (int i = 0; i < N; i++) {
        x[i] /= N;
        if (cabs(x[i]) < zero_threshold) x[i] = 0.0 + 0.0 * I;
    }
}

void iter_fft(double complex *x, int N) {
    x = padding(x, &N);
    inner_iter_fft(x, N, 0);
    for (int i = 0; i < N; i++) {
        if (cabs(x[i]) < zero_threshold) x[i] = 0.0 + 0.0 * I;
    }
}

void iter_ifft(double complex *x, int N) {
    inner_iter_fft(x, N, 1);
    for (int i = 0; i < N; i++) {
        x[i] /= N;
        if (cabs(x[i]) < zero_threshold) x[i] = 0.0 + 0.0 * I;
    }
}

void inner_recur_fft(double complex *x, int N, int inverse) {
    if (N <= 1) return;

    // Split
    double complex *even = (double complex *)malloc(N / 2 * sizeof(double complex));
    double complex *odd = (double complex *)malloc(N / 2 * sizeof(double complex));
    for (int i = 0; i < N / 2; i++) {
        even[i] = x[i * 2];
        odd[i] = x[i * 2 + 1];
    }

    // Conquer
    inner_recur_fft(even, N / 2, inverse);
    inner_recur_fft(odd, N / 2, inverse);

    // Combine
    for (int k = 0; k < N / 2; k++) {
        double angle = -2.0 * PI * k / N * (inverse ? -1 : 1);
        double complex twiddle = cexp(I * angle); // e^(-i2πk/N)
        x[k] = even[k] + twiddle * odd[k];
        x[k + N / 2] = even[k] - twiddle * odd[k];
    }
    free(even);
    free(odd);
}

void inner_iter_fft(double complex *x, int N, int inverse) {
    // Bit-reversal permutation
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j) {
            double complex temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
    }

    // Iterative FFT
    for (int len = 2; len <= N; len <<= 1) {
        double angle = -2.0 * PI / len * (inverse ? -1 : 1);
        double complex wlen = cexp(I * angle);
        for (int i = 0; i < N; i += len) {
            double complex w = 1.0 + 0.0 * I;
            for (int j = 0; j < len / 2; j++) {
                double complex u = x[i + j];
                double complex v = x[i + j + len / 2] * w;
                x[i + j] = u + v;
                x[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}