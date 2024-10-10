#include "divmod.c"

// 유리수 사칙연산만으로 나올 수 있는 수는 유리수로 한정된다.
// 분수 자료형을 만들어서 관리하면 이론 상 정밀도가 손실되지 않고 수를 관리할 수 있다.
typedef struct fraction {
    bigint *numer, *denom;
} fraction;

fraction* floatstr_to_fraction(char *str) {

}
fraction* bigint_to_fraction(bigint *n) {

}


fraction* FADD(fraction *a, fraction *b) {
    
}
fraction* FSUB(fraction *a, fraction *b) {

}
fraction* FMUL(fraction *a, fraction *b) {

}
fraction* FDIV(fraction *a, fraction *b) {

}

void reduce(fraction *n) {

}
void print_fraction(fraction *n, long long dec) {

}