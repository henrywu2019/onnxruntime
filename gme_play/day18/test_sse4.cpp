//
// Created by henry on 2/10/23.
//
#include <sein.hpp>
#include <immintrin.h>
#include <gamma_common.h>
#include "conv2d.h"
#include "mlas.h"


    /*
Dump of assembler code for function _Z3funPfS_i:
   0x0000555555555409 <+0>:	endbr64
   0x000055555555540d <+4>:	push   %rbp
   0x000055555555540e <+5>:	mov    %rsp,%rbp
   0x0000555555555411 <+8>:	and    $0xffffffffffffffe0,%rsp
   0x0000555555555415 <+12>:	sub    $0x100,%rsp
   0x000055555555541c <+19>:	mov    %rdi,0x18(%rsp)
   0x0000555555555421 <+24>:	mov    %rsi,0x10(%rsp)
   0x0000555555555426 <+29>:	mov    %edx,0xc(%rsp)
   0x000055555555542a <+33>:	mov    $0x68,%edi
   0x000055555555542f <+38>:	call   0x5555555551a0 <putchar@plt>
   0x0000555555555434 <+43>:	lea    0x2be5(%rip),%rax        # 0x555555558020 <INDEX_BASE>
   0x000055555555543b <+50>:	mov    %rax,0x38(%rsp)
   0x0000555555555440 <+55>:	mov    0x38(%rsp),%rax
   0x0000555555555445 <+60>:	vmovdqu (%rax),%ymm0
   0x0000555555555449 <+64>:	vmovdqa %ymm0,0x40(%rsp)
   0x000055555555544f <+70>:	mov    0xc(%rsp),%eax
   0x0000555555555453 <+74>:	mov    %eax,0x2c(%rsp)
   0x0000555555555457 <+78>:	vpbroadcastd 0x2c(%rsp),%ymm0
   0x000055555555545e <+85>:	vmovdqa %ymm0,%ymm1
   0x0000555555555462 <+89>:	vmovdqa 0x40(%rsp),%ymm0
   0x0000555555555468 <+95>:	vmovdqa %ymm0,0xc0(%rsp)
   0x0000555555555471 <+104>:	vmovdqa %ymm1,0xe0(%rsp)
   0x000055555555547a <+113>:	vmovdqa 0xc0(%rsp),%ymm1
   0x0000555555555483 <+122>:	vmovdqa 0xe0(%rsp),%ymm0
   0x000055555555548c <+131>:	vpmulld %ymm0,%ymm1,%ymm0
   0x0000555555555491 <+136>:	vmovdqa %ymm0,0x60(%rsp)
   0x0000555555555497 <+142>:	vxorps %xmm1,%xmm1,%xmm1
   0x000055555555549b <+146>:	vxorps %xmm0,%xmm0,%xmm0
   0x000055555555549f <+150>:	vcmpeqps %ymm1,%ymm0,%ymm0
   0x00005555555554a4 <+155>:	vmovdqa 0x60(%rsp),%ymm1
   0x00005555555554aa <+161>:	vxorps %xmm2,%xmm2,%xmm2
   0x00005555555554ae <+165>:	mov    0x18(%rsp),%rax
   0x00005555555554b3 <+170>:	vgatherdps %ymm0,(%rax,%ymm1,8),%ymm2
   0x00005555555554b9 <+176>:	vmovaps %ymm2,%ymm0
   0x00005555555554bd <+180>:	vmovaps %ymm0,0x80(%rsp)
   0x00005555555554c6 <+189>:	mov    0x10(%rsp),%rax
   0x00005555555554cb <+194>:	mov    %rax,0x30(%rsp)
   0x00005555555554d0 <+199>:	vmovaps 0x80(%rsp),%ymm0
   0x00005555555554d9 <+208>:	vmovaps %ymm0,0xa0(%rsp)
   0x00005555555554e2 <+217>:	mov    0x30(%rsp),%rax
   0x00005555555554e7 <+222>:	vmovaps 0xa0(%rsp),%ymm0
   0x00005555555554f0 <+231>:	vmovaps %ymm0,(%rax)
   0x00005555555554f4 <+235>:	nop
   0x00005555555554f5 <+236>:	mov    $0x68,%edi
   0x00005555555554fa <+241>:	call   0x5555555551a0 <putchar@plt>
   0x00005555555554ff <+246>:	nop
   0x0000555555555500 <+247>:	leave
   0x0000555555555501 <+248>:	ret
End of assembler dump.
 * */
static int INDEX_BASE[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

inline void fun_sse(float* src, float* dst, int GatherStride){
  auto idx = _mm_loadu_si128((__m128i*)INDEX_BASE);
  auto indices = _mm_mullo_epi32(idx, _mm_set1_epi32(GatherStride));
  auto src_vec = _mm_i32gather_ps(src, indices, 4);
  _mm_storeu_ps(dst, src_vec);
}

inline void fun_avx2(float* src, float* dst, int GatherStride){
  auto idx = _mm256_loadu_si256((__m256i*)INDEX_BASE);
  auto indices = _mm256_mullo_epi32(idx, _mm256_set1_epi32(GatherStride));
  auto src_vec = _mm256_i32gather_ps(src, indices, 8);
  _mm256_storeu_ps(dst, src_vec);
}

inline void fun_avx512(float* src, float* dst, int GatherStride){
  auto idx = _mm512_loadu_si512((__m512i*)INDEX_BASE);
  auto indices = _mm512_mullo_epi32(idx, _mm512_set1_epi32(GatherStride));
  auto src_vec = _mm512_i32gather_ps(indices, src, 16);
  _mm512_storeu_ps(dst, src_vec);
}

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
  auto v = _mm_load_ss(&S[0 * GatherStride]);
  for (int i=0;i<5000*5000;i++){
    v = _mm_insert_ps(v, _mm_load_ss(&S[1 * GatherStride]), 0x10);
    v = _mm_insert_ps(v, _mm_load_ss(&S[2 * GatherStride]), 0x20);
    v = _mm_insert_ps(v, _mm_load_ss(&S[3 * GatherStride]), 0x30);
    _mm_storeu_ps(D, v);
  }
#endif
#if 1
  for (int i=0;i<5000*5000;i++){
    auto src=S+i%(400*298);
    float f0 = src[0 * GatherStride];
    float f1 = src[1 * GatherStride];
    float f2 = src[2 * GatherStride];
    float f3 = src[3 * GatherStride];

    D[0] = f0;
    D[1] = f1;
    D[2] = f2;
    D[3] = f3;
  }

#else
  for (int i=0;i<5000*5000/2;i++){
    auto idx = _mm256_loadu_si256((__m256i*)INDEX_BASE);
    auto indices = _mm256_mullo_epi32(idx, _mm256_set1_epi32(GatherStride));
    auto src_vec = _mm256_i32gather_ps(src, indices, 8);
    _mm256_storeu_ps(src, src_vec);
  }

#endif
  auto t1 = std::chrono::high_resolution_clock::now();
  long long t = std::chrono::duration_cast<std::chrono::microseconds>((t1 - t0)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " us" << std::endl;
}


void avx512_copy(float* src, float* dst) {
#if 0
  // Load the 8 floats from src using _mm256_i32gather_ps
  auto idx=_mm256_set_epi64x(7 * 64, 6 * 64, 5 * 64, 4 * 64, 3 * 64, 2 * 64, 1 * 64, 0 * 64);
  auto vsrc = _mm256_i32gather_ps(src, idx, 1);

  // Store the 8 floats to src using _mm256_store_ps
  _mm256_store_ps(src, vsrc);
#endif
}

void print_m256i(auto v) {
  const int32_t* ptr = (const int32_t*) &v;
  printf("[%d, %d, %d, %d, %d, %d, %d, %d]\n",
         ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
}
void print_m128i(auto v) {
  const int32_t* ptr = (const int32_t*) &v;
  printf("[%d, %d, %d, %d]\n",
         ptr[0], ptr[1], ptr[2], ptr[3]);
}


void transpose_4x4(float *mat) {
  auto row0 = _mm_loadu_ps(&mat[0]);
  auto row1 = _mm_loadu_ps(&mat[4]);
  auto row2 = _mm_loadu_ps(&mat[8]);
  auto row3 = _mm_loadu_ps(&mat[12]);
  auto tmp0 = _mm_shuffle_ps(row0, row1, 0b01000100);
  auto tmp2 = _mm_shuffle_ps(row0, row1, 0b11101110);
  auto tmp1 = _mm_shuffle_ps(row2, row3, 0b01000100);
  auto tmp3 = _mm_shuffle_ps(row2, row3, 0b11101110);
  _mm_storeu_ps(&mat[0], _mm_shuffle_ps(tmp0, tmp1, 0b10001000));
  _mm_storeu_ps(&mat[4], _mm_shuffle_ps(tmp0, tmp1, 0b11011101));
  _mm_storeu_ps(&mat[8], _mm_shuffle_ps(tmp2, tmp3, 0b10001000));
  _mm_storeu_ps(&mat[12], _mm_shuffle_ps(tmp2, tmp3, 0b11011101));
}

void transpose_8x8(float *src) {
}

void ReorderInputNchw(const int64_t* input_shape, const float* S, float* D) {
  auto_profiler ap(__FUNCTION__ );
  const int64_t nchwc_block_size = static_cast<int64_t>(MlasNchwcGetBlockSize());
  int64_t batch_count = input_shape[0];
  int64_t channel_count = input_shape[1];
  int64_t nchwc_channel_count = (channel_count + nchwc_block_size - 1) & ~(nchwc_block_size - 1);
  int64_t spatial_count = input_shape[2] * input_shape[3];
  for (int64_t n = 0; n < batch_count; n++) {
    MlasReorderInputNchw(S, D, static_cast<size_t>(channel_count), static_cast<size_t>(spatial_count));
    S += spatial_count * channel_count;
    D += spatial_count * nchwc_channel_count;
  }
}

/*
reorder_NCHW_NCHWc8_avx2 : 47 us
reorder_NCHW_NCHWc8_base : 121 us
ReorderInputNchw : 83 us
reorder_NCHW_NCHWc16_avx2 : 26 us
reorder_NCHW_NCHWc16_base : 80 us
reorder_NCHW_NCHWc32_avx2 : 26 us
reorder_NCHW_NCHWc32_base : 78 us
*/

int main(int argc, char** argv){
#ifdef __AVX512F__
    printf("AVX-512 is supported on this platform.\n");
#else
  printf("AVX-512 is not supported on this platform.%ld\n", sizeof(float));
#endif

  conv_attr ca(1,256,10,10);
  auto input = make_conv2d_input(ca);

  auto new_input=(float*)_mm_malloc(sizeof(float) * ca.input_size, 32);
  auto new_input_v2=(float*)_mm_malloc(sizeof(float) * ca.input_size, 32);
  auto new_input_v3=(float*)_mm_malloc(sizeof(float) * ca.input_size, 32);

  reorder_NCHW_NCHWc8_avx2(input,new_input,ca);
  reorder_NCHW_NCHWc8_base(input,new_input_v2,ca);
  int64_t InputShape[] = {int64_t(ca.N), int64_t(1) * int64_t(ca.C), int64_t(ca.H), int64_t(ca.W)};
  ReorderInputNchw(InputShape, input, new_input_v3);
  assert(array_equal(new_input, new_input_v2, ca.input_size));
  assert(array_equal(new_input, new_input_v3, ca.input_size));

  ::memset((void*)new_input,0,ca.input_size* sizeof(float));
  ::memset((void*)new_input_v2,0,ca.input_size* sizeof(float));
  reorder_NCHW_NCHWc16_avx2(input,new_input,ca);
  reorder_NCHW_NCHWc16_base(input,new_input_v2,ca);
  assert(array_equal(new_input, new_input_v2, ca.input_size));

  ::memset((void*)new_input,0,ca.input_size* sizeof(float));
  ::memset((void*)new_input_v2,0,ca.input_size* sizeof(float));
  reorder_NCHW_NCHWc32_avx2(input,new_input,ca);
  reorder_NCHW_NCHWc32_base(input,new_input_v2,ca);
  assert(array_equal(new_input, new_input_v2, ca.input_size));
  return 0;

  float* X=(float*)_mm_malloc(sizeof(float) * 16, 32);
  REP(i,0,16) X[i]=i;
  transpose_4x4(X);


  float* Y=(float*)_mm_malloc(sizeof(float) * 64, 32);
  REP(i,0,64) Y[i]=i;
  transpose_8x8(Y);

  print_matrix(X, 4, 4);
  print_matrix(Y, 8, 8);

  _mm_free(X);
  _mm_free(Y);



  float* I=(float*)_mm_malloc(sizeof(float) * 400*298*256, 32);
  float* O=(float*)_mm_malloc(sizeof(float) * 400*298*256, 32);
  REP(i,0,400*298*256) O[i]=i;

  //fun_sse(O, I, 64);
  /*
=> 0x00005555555553d0 <+161>:	mov    0x90(%rsp),%rax
   0x00005555555553d8 <+169>:	add    $0x1c,%rax
   0x00005555555553dc <+173>:	vmovss (%rax),%xmm0
   0x00005555555553e0 <+177>:	mov    0x90(%rsp),%rax
   0x00005555555553e8 <+185>:	add    $0x18,%rax
   0x00005555555553ec <+189>:	vmovss (%rax),%xmm1
   0x00005555555553f0 <+193>:	mov    0x90(%rsp),%rax
   0x00005555555553f8 <+201>:	add    $0x14,%rax
   0x00005555555553fc <+205>:	vmovss (%rax),%xmm2
   0x0000555555555400 <+209>:	mov    0x90(%rsp),%rax
   0x0000555555555408 <+217>:	add    $0x10,%rax
   0x000055555555540c <+221>:	vmovss (%rax),%xmm3
   0x0000555555555410 <+225>:	mov    0x90(%rsp),%rax
   0x0000555555555418 <+233>:	add    $0xc,%rax
   0x000055555555541c <+237>:	vmovss (%rax),%xmm4
   0x0000555555555420 <+241>:	mov    0x90(%rsp),%rax
   0x0000555555555428 <+249>:	add    $0x8,%rax
   0x000055555555542c <+253>:	vmovss (%rax),%xmm5
   0x0000555555555430 <+257>:	mov    0x90(%rsp),%rax
   0x0000555555555438 <+265>:	add    $0x4,%rax
   0x000055555555543c <+269>:	vmovss (%rax),%xmm6
   0x0000555555555440 <+273>:	mov    0x90(%rsp),%rax
   0x0000555555555448 <+281>:	vmovss (%rax),%xmm7
   0x000055555555544c <+285>:	vmovss %xmm7,0x28(%rsp)
   0x0000555555555452 <+291>:	vmovss %xmm6,0x2c(%rsp)
   0x0000555555555458 <+297>:	vmovss %xmm5,0x30(%rsp)
   0x000055555555545e <+303>:	vmovss %xmm4,0x34(%rsp)
   0x0000555555555464 <+309>:	vmovss %xmm3,0x38(%rsp)
   0x000055555555546a <+315>:	vmovss %xmm2,0x3c(%rsp)
   0x0000555555555470 <+321>:	vmovss %xmm1,0x40(%rsp)
   0x0000555555555476 <+327>:	vmovss %xmm0,0x44(%rsp)
   0x000055555555547c <+333>:	vmovss 0x44(%rsp),%xmm0
   0x0000555555555482 <+339>:	vmovss %xmm0,0x48(%rsp)
   0x0000555555555488 <+345>:	vmovss 0x40(%rsp),%xmm0
   0x000055555555548e <+351>:	vmovss %xmm0,0x4c(%rsp)
   0x0000555555555494 <+357>:	vmovss 0x3c(%rsp),%xmm0
   0x000055555555549a <+363>:	vmovss %xmm0,0x50(%rsp)
   0x00005555555554a0 <+369>:	vmovss 0x38(%rsp),%xmm0
   0x00005555555554a6 <+375>:	vmovss %xmm0,0x54(%rsp)
   0x00005555555554ac <+381>:	vmovss 0x34(%rsp),%xmm0
   0x00005555555554b2 <+387>:	vmovss %xmm0,0x58(%rsp)
   0x00005555555554b8 <+393>:	vmovss 0x30(%rsp),%xmm0
   0x00005555555554be <+399>:	vmovss %xmm0,0x5c(%rsp)
   0x00005555555554c4 <+405>:	vmovss 0x2c(%rsp),%xmm0
   0x00005555555554ca <+411>:	vmovss %xmm0,0x60(%rsp)
   0x00005555555554d0 <+417>:	vmovss 0x28(%rsp),%xmm0
   0x00005555555554d6 <+423>:	vmovss %xmm0,0x64(%rsp)
   0x00005555555554dc <+429>:	vmovss 0x48(%rsp),%xmm1
   0x00005555555554e2 <+435>:	vmovss 0x4c(%rsp),%xmm0
   0x00005555555554e8 <+441>:	vunpcklps %xmm1,%xmm0,%xmm1
   0x00005555555554ec <+445>:	vmovss 0x50(%rsp),%xmm2
   0x00005555555554f2 <+451>:	vmovss 0x54(%rsp),%xmm0
   0x00005555555554f8 <+457>:	vunpcklps %xmm2,%xmm0,%xmm0
   0x00005555555554fc <+461>:	vmovlhps %xmm1,%xmm0,%xmm1
   0x0000555555555500 <+465>:	vmovss 0x58(%rsp),%xmm2
   0x0000555555555506 <+471>:	vmovss 0x5c(%rsp),%xmm0
   0x000055555555550c <+477>:	vunpcklps %xmm2,%xmm0,%xmm2
   0x0000555555555510 <+481>:	vmovss 0x60(%rsp),%xmm3
   0x0000555555555516 <+487>:	vmovss 0x64(%rsp),%xmm0
   0x000055555555551c <+493>:	vunpcklps %xmm3,%xmm0,%xmm0
   0x0000555555555520 <+497>:	vmovlhps %xmm2,%xmm0,%xmm0
   0x0000555555555524 <+501>:	vinsertf128 $0x1,%xmm1,%ymm0,%ymm0
   0x000055555555552a <+507>:	nop
   0x000055555555552b <+508>:	vmovaps %ymm0,0xc0(%rsp)
   0x0000555555555534 <+517>:	mov    0x88(%rsp),%rax
   0x000055555555553c <+525>:	mov    %rax,0xb8(%rsp)
   0x0000555555555544 <+533>:	vmovaps 0xc0(%rsp),%ymm0
   0x000055555555554d <+542>:	vmovaps %ymm0,0xe0(%rsp)
   0x0000555555555556 <+551>:	vmovaps 0xe0(%rsp),%ymm0
   0x000055555555555f <+560>:	mov    0xb8(%rsp),%rax
   0x0000555555555567 <+568>:	vmovups %ymm0,(%rax)
   * */
  //////////////////////////////////////////////
  /*auto a = _mm256_set_ps(O[0], O[1], O[2], O[3], O[4], O[5], O[6], O[7]);
  //auto b = _mm_setr_ps(9.0f, 10.0f, 11.0f, 12.0f);
  // insert the four single-precision values in b into the upper 128 bits of a
  //auto c = _mm256_insertf128_ps(a, b, 0);
  _mm256_storeu_ps(I, a);*/
  //////////////////////////////////
  /*
  0x00005555555553b3 <+132>:	mov    -0x30(%rbp),%rax
  0x00005555555553b7 <+136>:	vmovss (%rax),%xmm0
  0x00005555555553bb <+140>:	mov    -0x38(%rbp),%rax
  0x00005555555553bf <+144>:	vmovss %xmm0,(%rax)
  0x00005555555553c3 <+148>:	mov    -0x38(%rbp),%rax
  0x00005555555553c7 <+152>:	add    $0x4,%rax
  0x00005555555553cb <+156>:	mov    -0x30(%rbp),%rdx
  0x00005555555553cf <+160>:	vmovss 0x4(%rdx),%xmm0
  0x00005555555553d4 <+165>:	vmovss %xmm0,(%rax)
  0x00005555555553d8 <+169>:	mov    -0x38(%rbp),%rax
  0x00005555555553dc <+173>:	add    $0x8,%rax
  0x00005555555553e0 <+177>:	mov    -0x30(%rbp),%rdx
  0x00005555555553e4 <+181>:	vmovss 0x8(%rdx),%xmm0
  0x00005555555553e9 <+186>:	vmovss %xmm0,(%rax)
  0x00005555555553ed <+190>:	mov    -0x38(%rbp),%rax
  0x00005555555553f1 <+194>:	add    $0xc,%rax
  0x00005555555553f5 <+198>:	mov    -0x30(%rbp),%rdx
  0x00005555555553f9 <+202>:	vmovss 0xc(%rdx),%xmm0
  0x00005555555553fe <+207>:	vmovss %xmm0,(%rax)
  0x0000555555555402 <+211>:	mov    -0x38(%rbp),%rax
  0x0000555555555406 <+215>:	add    $0x10,%rax
  0x000055555555540a <+219>:	mov    -0x30(%rbp),%rdx
  0x000055555555540e <+223>:	vmovss 0x10(%rdx),%xmm0
  0x0000555555555413 <+228>:	vmovss %xmm0,(%rax)
  0x0000555555555417 <+232>:	mov    -0x38(%rbp),%rax
  0x000055555555541b <+236>:	add    $0x14,%rax
  0x000055555555541f <+240>:	mov    -0x30(%rbp),%rdx
  0x0000555555555423 <+244>:	vmovss 0x14(%rdx),%xmm0
  0x0000555555555428 <+249>:	vmovss %xmm0,(%rax)
  0x000055555555542c <+253>:	mov    -0x38(%rbp),%rax
  0x0000555555555430 <+257>:	add    $0x18,%rax
  0x0000555555555434 <+261>:	mov    -0x30(%rbp),%rdx
  0x0000555555555438 <+265>:	vmovss 0x18(%rdx),%xmm0
  0x000055555555543d <+270>:	vmovss %xmm0,(%rax)
  0x0000555555555441 <+274>:	mov    -0x38(%rbp),%rax
  0x0000555555555445 <+278>:	add    $0x1c,%rax
  0x0000555555555449 <+282>:	mov    -0x30(%rbp),%rdx
  0x000055555555544d <+286>:	vmovss 0x1c(%rdx),%xmm0
  0x0000555555555452 <+291>:	vmovss %xmm0,(%rax)
  */
  //I[0]=O[0], I[1]=O[1], I[2]=O[2], I[3]=O[3], I[4]=O[4], I[5]=O[5], I[6]=O[6];
  //I[7]=O[7];
  printf("o");


  MlasReorderGatherFloat32x4(I, O, 128);
  _mm_free(I), _mm_free(O);
}