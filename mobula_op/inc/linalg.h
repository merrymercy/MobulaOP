#ifndef _MOBULA_LINALG_H_
#define _MOBULA_LINALG_H_

#include "defines.h"
#include "context/context.h"

namespace mobula {

// out[i, j] = sum(a[i, :] * b[:, j])
template <typename T>
MOBULA_KERNEL linalg_gemm_ff_kernel(const int n, const T *a, const T *b, const int U, const int J, T *out) {
    parfor(n, [&](int i) {
        for (int u = 0; u < U; ++u) {
            for (int j = 0; j < J; ++j) {
                out[i * J + j] += a[i * U + u] * b[u * J + j];
            }
        }
    });
}

// out[i, j] = sum(a[i, :] * b[j, :])
template <typename T>
MOBULA_KERNEL linalg_gemm_ft_kernel(const int n, const T *a, const T *b, const int U, const int J, T *out) {
    parfor(n, [&](int i) {
        for (int j = 0; j < J; ++j) {
            for (int u = 0; u < U; ++u) {
                out[i * J + j] += a[i * U + u] * b[j * U + u];
            }
        }
    });
}

// out[i, j] = sum(a[:, i] * b[:, j])
template <typename T>
MOBULA_KERNEL linalg_gemm_tf_kernel(const int n, const T *a, const T *b, const int I, const int J, T *out) {
    parfor(n, [&](int u) {
        for (int i = 0; i < I; ++i) {
            for (int j = 0; j < J; ++j) {
                out[i * J + j] += a[u * I + i] * b[u * J + j];
            }
        }
    });
}

// out[i, j] = sum(a[:, i] * b[j, :])
template <typename T>
MOBULA_KERNEL linalg_gemm_tt_kernel(const int n, const T *a, const T *b, const int I, const int U, const int J, T *out) {
    parfor(n, [&](int j) {
        for (int u = 0; u < U; ++u) {
            for (int i = 0; i < I; ++i) {
                out[i * J + j] += a[u * I + i] * b[j * U + u];
            }
        }
    });
}

}

extern "C" {
using namespace mobula;

void linalg_gemm_ff(const DType *a, const DType *b, const int I, const int U, const int J, DType *out) {
    const int N = I;
    KERNEL_RUN(linalg_gemm_ff_kernel<DType>, N)(N, a, b, U, J, out);
}

void linalg_gemm_ft(const DType *a, const DType *b, const int I, const int U, const int J, DType *out) {
    const int N = I;
    KERNEL_RUN(linalg_gemm_ft_kernel<DType>, N)(N, a, b, U, J, out);
}

void linalg_gemm_tf(const DType *a, const DType *b, const int I, const int U, const int J, DType *out) {
    const int N = U;
    KERNEL_RUN(linalg_gemm_tf_kernel<DType>, N)(N, a, b, I, J, out);
}

void linalg_gemm_tt(const DType *a, const DType *b, const int I, const int U, const int J, DType *out) {
    const int N = J;
    KERNEL_RUN(linalg_gemm_tt_kernel<DType>, N)(N, a, b, I, U, J, out);
}

}

#endif
