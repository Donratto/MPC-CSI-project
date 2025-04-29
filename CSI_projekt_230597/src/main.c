#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <complex.h> // For double complex
#include "moj_fft.c"

// #define DEBUG
#define FILEOUTPUT

int main() {
    float d = 2; // Signal duration [s]
    int f = 1000; // Signal frequency [Hz]
    int fs = 4096; // Sampling frequency [Hz]
    int N = d * fs; // Number of points
    org_N = N;
    clock_t start, end;
    double time_taken;

    // File output
    #ifdef FILEOUTPUT
    #ifdef win32
        char *file_name_fft_recur = "out\\fft_recur.txt";
        char *file_name_fft_iter = "out\\fft_iter.txt";
        char *file_name_ifft_recur = "out\\ifft_recur.txt";
        char *file_name_ifft_iter = "out\\ifft_iter.txt";
        char *file_name_debug = "out\\debug.txt";
        char *file_name_gen = "out\\gen.txt";
        char *file_name_stats = "out\\stats.txt";
    #else
        char *file_name_fft_recur = "out/fft_recur.txt";
        char *file_name_fft_iter = "out/fft_iter.txt";
        char *file_name_ifft_recur = "out/ifft_recur.txt";
        char *file_name_ifft_iter = "out/ifft_iter.txt";
        char *file_name_debug = "out/debug.txt";
        char *file_name_gen = "out/gen.txt";
        char *file_name_stats = "out/stats.txt";
    #endif

    FILE *file_fft_recur = fopen(file_name_fft_recur, "w");
    FILE *file_fft_iter = fopen(file_name_fft_iter, "w");
    FILE *file_ifft_recur = fopen(file_name_ifft_recur, "w");
    FILE *file_ifft_iter = fopen(file_name_ifft_iter, "w");
    FILE *file_stats = fopen(file_name_stats, "w");
    FILE *gen = fopen(file_name_gen, "w");
    #ifdef DEBUG
    FILE *file_debug = fopen(file_name_debug, "w");
    #endif

    if (!file_fft_recur || !file_fft_iter || !file_ifft_recur || !file_ifft_iter || !file_stats || !gen
        #ifdef DEBUG
        || !file_debug
        #endif
    ) {
        fprintf(stderr, "Error opening one or more files.\n");
        return 1;
    }
    #endif

    // Allocate memory for input data
    double complex *x = (double complex *)malloc(N * sizeof(double complex));
    if (x == NULL) {
        fprintf(stderr, "Memory allocation failed for x.\n");
        return 1;
    }

    // Initialize input data
    for (int i = 0; i < N; i++) {
        x[i] = sin(2 * M_PI * f * (d / (N - 1)) * i) + I * 0;
        #ifdef FILEOUTPUT
        fprintf(gen, "%.97f\n", creal(x[i]));
        #ifdef DEBUG
        if (i == 0) {
            fprintf(file_debug, "#1\tGenerated data:\n");
        }
        fprintf(file_debug, "[%d] = %f %c %fi\n", i, creal(x[i]), (cimag(x[i]) < 0 ? '-' : '+'), fabs(cimag(x[i])));
        if (i == N - 1) fclose(file_debug);
        #endif
        #endif
    }

    // If N is not a power of 2, pad with zeros
    x = padding(x, &N);

    // Allocate memory for FFT results
    double complex *X_recur = (double complex *)malloc(N * sizeof(double complex));
    double complex *X_iter = (double complex *)malloc(N * sizeof(double complex));
    if (X_recur == NULL || X_iter == NULL) {
        fprintf(stderr, "Memory allocation failed for FFT results.\n");
        free(x);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        X_iter[i] = x[i];
        X_recur[i] = x[i];
    }

    // Perform FFT
    start = clock();
    recur_fft(X_recur, N);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(file_stats, "recur FFT,%f\n", time_taken);

    start = clock();
    iter_fft(X_iter, N);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(file_stats, "iter FFT,%f\n", time_taken);

    #ifdef FILEOUTPUT
    for (int i = 0; i < N; i++) {
        fprintf(file_fft_recur, "%f,%f\n", creal(X_recur[i]), cimag(X_recur[i]));
        fprintf(file_fft_iter, "%f,%f\n", creal(X_iter[i]), cimag(X_iter[i]));
    }
    #endif

    // Perform IFFT
    start = clock();
    recur_ifft(X_recur, N);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(file_stats, "recur IFFT,%f\n", time_taken);

    start = clock();
    iter_ifft(X_iter, N);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(file_stats, "iter IFFT,%f\n", time_taken);

    // Restore original size
    N = org_N;

    #ifdef FILEOUTPUT
    for (int i = 0; i < N; i++) {
        fprintf(file_ifft_recur, "%f,%f\n", creal(X_recur[i]), cimag(X_recur[i]));
        fprintf(file_ifft_iter, "%f,%f\n", creal(X_iter[i]), cimag(X_iter[i]));
    }
    #endif

    // Free allocated memory
    free(x);
    free(X_recur);
    free(X_iter);

    #ifdef FILEOUTPUT
    fclose(file_fft_recur);
    fclose(file_fft_iter);
    fclose(file_ifft_recur);
    fclose(file_ifft_iter);
    fclose(file_stats);
    fclose(gen);
    #ifdef DEBUG
    fclose(file_debug);
    #endif
    #endif

    return 0;
}
