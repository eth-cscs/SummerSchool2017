#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include "util.h"


// Conventions: A -> MxK, B -> KxN, C -> MxN, all row major

template<typename T>
void dgemm_naive(size_t M, size_t N, size_t K,
                 T alpha, const T *A, const T *B, T beta, T *C)
{
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            T prod = T{0};
            for (size_t k = 0; k < K; ++k) {
                prod += A[i*K+k]*B[k*N+j];
            }

            C[i*N+j] = alpha*prod + beta*C[i*N+j];
        }
    }
}

template<typename T>
void dgemm_lreorder(size_t M, size_t N, size_t K,
                    T alpha, const T *A, const T *B, T beta, T *C)
{
    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            T prod = T{0};
            for (size_t j = 0; j < N; ++j) {
                prod += A[i*K+k]*B[k*N+j];
            }

            C[i*N+k] = alpha*prod + beta*C[i*N+k];
        }
    }
}

template<typename T>
void dgemm_omp(size_t M, size_t N, size_t K,
               T alpha, const T *A, const T *B, T beta, T *C)
{
    #pragma omp parallel for collapse(2) firstprivate(alpha, beta)
    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            T prod = T{0};
            for (size_t j = 0; j < N; ++j) {
                prod += A[i*K+k]*B[k*N+j];
            }

            C[i*N+k] = alpha*prod + beta*C[i*N+k];
        }
    }
}

template<typename T>
void dgemm_openacc(size_t M, size_t N, size_t K,
                   T alpha, const T *A, const T *B, T beta, T *C)
{
    // TODO: offload the following loops to gpu
    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            T prod = T{0};
            for (size_t j = 0; j < N; ++j) {
                prod += A[i*K+k]*B[k*N+j];
            }

            C[i*N+k] = alpha*prod + beta*C[i*N+k];
        }
    }
}

template<typename T>
struct cublas_traits {
    using gemm_type = std::function<cublasStatus_t(
        cublasHandle_t, cublasOperation_t, cublasOperation_t, int, int, int,
        const T*, const T*, int, const T*, int, const T*, T*, int)>;
};

template<typename T>
typename cublas_traits<T>::gemm_type gemm_fn();


template<>
typename cublas_traits<double>::gemm_type gemm_fn<double>()
{
    return cublasDgemm;
}

template<>
typename cublas_traits<float>::gemm_type gemm_fn<float>()
{
    return cublasSgemm;
}


template<typename T>
void dgemm_cublas(size_t M, size_t N, size_t K,
                  T alpha, const T *A, const T *B, T beta, T *C)
{
    // Setup cublas
    cublasHandle_t handle;
    if (cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS) {
        std::cerr << "CUBLAS initialization failed\n";
        exit(1);
    }

    auto cublas_gemm = gemm_fn<T>();

    // TODO: Move the data to the device
    {
        // TODO: Get the device pointers
        {
            if (cublas_gemm(handle, CUBLAS_OP_T, CUBLAS_OP_T,
                            M, N, K, &alpha, A, K, B, N, &beta, C, N) !=
                CUBLAS_STATUS_SUCCESS) {
                std::cerr << "CUBLAS GEMM failed\n";
                exit(1);
            }
        }

#ifdef __PGI
        // Workaround for PGI 16.9
        #pragma acc update host(C[0:M*N])
#endif
    }

    cublasDestroy(handle);
}


template<typename T>
void check_result(size_t M, size_t N, const T *A, const T *B,
                  const char *msg = nullptr)
{
    if (msg) {
        std::cout << "Checking result " << "(" << msg << ")" << " ... ";
    } else {
        std::cout << "Checking result ... ";
    }

    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (std::fabs(A[i*N+j] - B[i*N+j]) > 1e-15) {
                std::cout << "failed\n";
                std::cout << A[i*N+j] << " != " << B[i*N+j] << "\n";
                return;
            }
        }
    }

    std::cout << "success\n";
}


template<
    typename T,
    typename F = std::function<
        void(size_t, size_t, size_t, T, const T*, const T*, T, T*)
    >
>
std::pair<std::unique_ptr<T>, double>
run_benchmark(size_t M, size_t N, size_t K, const T *A, const T *B,
                  F dgemm_fn, const char *name, const T *Cref = nullptr)
{
    auto C = std::unique_ptr<T>(malloc_host<T>(M*N, .2));
    auto start = get_time();
    dgemm_fn(M, N, K, 2., A, B, 1., C.get());
    auto time_dgemm = get_time() - start;
    if (Cref) {
        check_result(N, M, Cref, C.get(), name);
    }

    return std::make_pair(std::move(C), time_dgemm);
}


int main(int argc, char **argv)
{
    using value_type = double;

    size_t pow = read_arg(argc, argv, 1, 8);
    size_t n   = 1 << pow;

    std::cout << "Running dgemm benchmark for " << n << "x" << n
              << " arrays (total working set size: "
              << (3*n*n + 2*n)*sizeof(value_type)*1.e-6 << " MB)\n";

    auto a = malloc_host<value_type>(n*n, 1.);
    auto b = malloc_host<value_type>(n*n, 1.);

    // Compute a reference result
    auto ref = run_benchmark(n, n, n, a, b, dgemm_lreorder<value_type>,
                             "loop reordering");
    auto cref = std::move(ref.first);
    auto time_dgemm_lreorder = ref.second;

    // Run the naive kernel
#if !defined(NO_NAIVE)
    auto time_dgemm_naive = run_benchmark(
        n, n, n, a, b, dgemm_naive<value_type>, "naive", cref.get()).second;
#endif

    // Run the OpenMP kernel
    auto time_dgemm_omp = run_benchmark(
        n, n, n, a, b, dgemm_omp<value_type>, "OpenMP", cref.get()).second;

    // Run the OpenACC kernel
    auto time_dgemm_openacc = run_benchmark(
        n, n, n, a, b, dgemm_openacc<value_type>, "OpenACC", cref.get()).second;

    // Run the CUBLAS kernel
    auto time_dgemm_cublas = run_benchmark(
        n, n, n, a, b, dgemm_cublas<value_type>, "CUBLAS", cref.get()).second;


    std::cout << "-------\ntimings\n-------\n";
#if !defined(NO_NAIVE)
    std::cout << "dgemm naive (serial): " << time_dgemm_naive << " s\n";
#endif
    std::cout << "dgemm lreorder (serial): " << time_dgemm_lreorder << " s\n";
    std::cout << "dgemm lreorder (multicore): " << time_dgemm_omp << " s\n";
    std::cout << "dgemm lreorder (gpu): " << time_dgemm_openacc << " s\n";
    std::cout << "dgemm CUBLAS: " << time_dgemm_cublas << " s\n";
    return 0;
}
