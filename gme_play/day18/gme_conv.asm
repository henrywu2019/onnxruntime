global _gme_conv

; _gme_conv(I,F,O,oh,ow,kh,kw,tunable_x,c_)
;/////////////////////////////////////////////////////////
;        REP(l,0,ca.OW){
;          REP(t,0,reg_n) pr[t]=_mm256_xor_ps(pr[t],pr[t]);
;          __m256 x{}, y{};
;          REP(n,0,ca.R){
;            REP(o,0,ca.L){
;              REP(m,0,tunable_x){ // channel dim
;                int i_offset = input_index_new(0,j,k+n,l+o,m);
;                x = _mm256_set1_ps(core[i_offset]);
;                REP(p,0,reg_n){
;                  int f_offset = filter_index_new((i*reg_n+p),j,n,o,m,0);
;                  y = _mm256_load_ps(f+f_offset);
;                  pr[p] = _mm256_fmadd_ps(x, y, pr[p]);
;                }
;              }
;            }
;          }
;          REP(q,0,reg_n){
;            int o_offset = output_index_nchwc(i,k,l,q*VEC_LEN);
;            auto tmp = _mm256_loadu_ps(output+o_offset);
;            pr[q] = _mm256_add_ps(pr[q], tmp);
;            _mm256_storeu_ps(output+o_offset, pr[q]);
;          }
;/////////////////////////////////////////////////////////
section .text

; rdx - F, rcx - ih, r8 - len
_gme_conv:
