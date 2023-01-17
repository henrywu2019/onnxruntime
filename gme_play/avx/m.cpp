#include <iostream>
#include <immintrin.h> //AVX2 header

const int N = 3; // size of the matrix

void matrix_scalar_multiplication_AVX2(int matrix[N][N], int scalar) {
    __m256i s = _mm256_set1_epi32(scalar);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j += 8) {
            __m256i m = _mm256_loadu_si256((__m256i*)&matrix[i][j]);
            m = _mm256_mullo_epi32(m, s);
            _mm256_storeu_si256((__m256i*)&matrix[i][j], m);
        }
    }
}

void matrix_scalar_multiplication_AVX2_v2(float matrix[N][N], float scalar) {
    __m256 s = _mm256_set1_ps(scalar);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j += 8) {
            __m256 m = _mm256_loadu_ps(&matrix[i][j]);
            m = _mm256_mul_ps(m, s);
            _mm256_storeu_ps(&matrix[i][j], m);
        }
    }
}


int main() {
    //int matrix[N][N] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    //int scalar = 2;
    float matrix[N][N] = { {1.1, 2.2, 3.3}, {4.4, 5.5, 6.6}, {7.7, 8.8, 9.9} };
    float scalar = 2.0;

    std::cout << "Original matrix:" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    matrix_scalar_multiplication_AVX2_v2(matrix, scalar);

    std::cout << "Scaled matrix:" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

