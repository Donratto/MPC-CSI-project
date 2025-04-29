#include "mex.h"
#include "moj_fft.h" // Include your FFT library
#include <string.h>  // Include string.h for strcmp
#include <complex.h> // Use standard double complex type

// MEX entry point
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // Check input arguments
    if (nrhs != 2) {
        mexErrMsgIdAndTxt("moj_fft:invalidNumInputs", "Two inputs required: signal and FFT type.");
    }
    if (!mxIsDouble(prhs[0])) {
        mexErrMsgIdAndTxt("moj_fft:invalidInputType", "Input signal must be a double array.");
    }
    if (!mxIsChar(prhs[1])) {
        mexErrMsgIdAndTxt("moj_fft:invalidInputType", "FFT type must be a string ('recur', 'iter', 'recur_ifft', or 'iter_ifft').");
    }

    // Get input signal
    mwSize N = mxGetNumberOfElements(prhs[0]);
    double *real_part = mxGetPr(prhs[0]);
    double *imag_part = mxIsComplex(prhs[0]) ? mxGetPi(prhs[0]) : NULL;

    // Allocate memory for double complex array
    double complex *x = (double complex *)malloc(N * sizeof(double complex));
    if (x == NULL) {
        mexErrMsgIdAndTxt("moj_fft:memoryAllocation", "Memory allocation failed.");
    }

    // Copy input data into double complex array
    for (mwSize i = 0; i < N; i++) {
        if (imag_part) {
            x[i] = real_part[i] + imag_part[i] * I; // Complex input
        } else {
            x[i] = real_part[i] + 0.0 * I; // Real input
        }
    }

    // Get FFT type
    char fft_type[20];
    mxGetString(prhs[1], fft_type, sizeof(fft_type));

    // Perform FFT or IFFT
    if (strcmp(fft_type, "recur") == 0) {
        recur_fft(x, N);
    } else if (strcmp(fft_type, "iter") == 0) {
        iter_fft(x, N);
    } else if (strcmp(fft_type, "recur_ifft") == 0) {
        recur_ifft(x, N);
    } else if (strcmp(fft_type, "iter_ifft") == 0) {
        iter_ifft(x, N);
    } else {
        free(x);
        mexErrMsgIdAndTxt("moj_fft:invalidFFTType", "FFT type must be 'recur', 'iter', 'recur_ifft', or 'iter_ifft'.");
    }

    // Create output array
    plhs[0] = mxCreateDoubleMatrix(N, 1, mxCOMPLEX);
    double *out_real = mxGetPr(plhs[0]);
    double *out_imag = mxGetPi(plhs[0]);

    // Copy FFT result into output array
    for (mwSize i = 0; i < N; i++) {
        out_real[i] = creal(x[i]);
        out_imag[i] = cimag(x[i]);
    }

    // Free allocated memory
    free(x);
}