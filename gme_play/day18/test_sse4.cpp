//
// Created by henry on 2/10/23.
//
#include <bits/stdc++.h>
#include "immintrin.h"

__attribute__ ((always_inline)) inline
void
    MlasReorderGatherFloat32x4(
        const float* S,
        float* D,
        size_t GatherStride
        )
    /*++

    Routine Description:

    This routine gathers floats from the source buffer and writes a vector to
    the destination buffer.

    Arguments:

    S - Supplies the address of the source buffer.

      D - Supplies the address of the destination buffer.

      GatherStride - Supplies the stride to read elements from the source buffer.

          Return Value:

    None.

    --*/
{
  auto t0 = std::chrono::high_resolution_clock::now();
//#if defined(__SSE4_2__)
#if 0
  __m128 v = _mm_load_ss(&S[0 * GatherStride]);
  for (int i=0;i<5000*5000;i++){
    v = _mm_insert_ps(v, _mm_load_ss(&S[1 * GatherStride]), 0x10);
    v = _mm_insert_ps(v, _mm_load_ss(&S[2 * GatherStride]), 0x20);
    v = _mm_insert_ps(v, _mm_load_ss(&S[3 * GatherStride]), 0x30);
    _mm_storeu_ps(D, v);
  }
#else
  for (int i=0;i<5000*5000;i++){
    float f0 = S[0 * GatherStride];
    float f1 = S[1 * GatherStride];
    float f2 = S[2 * GatherStride];
    float f3 = S[3 * GatherStride];

    D[0] = f0;
    D[1] = f1;
    D[2] = f2;
    D[3] = f3;
  }

#endif
  auto t1 = std::chrono::high_resolution_clock::now();
  long long t = std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - t0)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " ns" << std::endl;
}

int main(int argc, char** argv){
  float* I=(float*)_mm_malloc(sizeof(float) * 1024, 32);
  float* O=(float*)_mm_malloc(sizeof(float) * 1024, 32);
  MlasReorderGatherFloat32x4(I, O, 128);
  _mm_free(I), _mm_free(O);
}