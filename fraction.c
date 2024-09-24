#include "multiply.c"

// 사칙연산만으로 나올 수 있는 수는 유리수로 한정된다.
// 분수 자료형을 만들어서 관리하면 이론상 정밀도가 손실되지 않고 수를 관리할 수 있다.
typedef struct fraction {
    bigint *numer, *denom;
} fraction;

// 추가 과제 : str_to_bigint 처럼 floatstr_to_fraction이 필요할 듯?

void bigint_to_fraction() {

}