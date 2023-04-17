#include <stdio.h>
#include <inttypes.h>

extern "C" int64_t _gamma_tn();
//extern void m();
int32_t gamma_tn(){
    static int32_t r = -1;
    if (r > 0) return r;
    return r=int32_t(_gamma_tn());
}

int main() {
//m();
    int64_t t = _gamma_tn();
    printf("%ld\n", t);
    return 0;
}



