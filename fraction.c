#include "divmod.c"

// 유리수 사칙연산만으로 나올 수 있는 수는 유리수로 한정된다.
// 분수 자료형을 만들어서 관리하면 이론 상 정밀도가 손실되지 않고 수를 관리할 수 있다.
typedef struct fraction {
    // numer : 분자, denom : 분모
    bigint *numer, *denom;
} fraction;

fraction* new_fraction() {
    return (fraction*)malloc(sizeof(fraction));
}

fraction* bigint_to_fraction(bigint *n) {
    fraction *f = new_fraction();
    f->numer = n;
    f->denom = str_to_bigint("1");
    return f;
}

void free_fraction(fraction *f) {
    free_bigint(f->numer);
    free_bigint(f->denom);
    free(f);
}

void reduce(fraction *f) {
    bigint *g = GCD(f->numer, f->denom);
    bigint *d1 = NULL, *m1 = NULL;
    bigint *d2 = NULL, *m2 = NULL;
    naiveDIVMOD(f->numer, g, &d1, &m1);
    naiveDIVMOD(f->denom, g, &d2, &m2);

    free_bigint(m1);
    free_bigint(m2);
    free_bigint(g);
    free_bigint(f->numer);
    free_bigint(f->denom);

    f->numer = d1;
    f->denom = d2;

    // 분자가 부호 정보를 담도록.
    bool sgn = f->denom->sign ^ f->numer->sign;
    if (sgn==MINUS) f->denom->sign = PLUS, f->numer->sign = MINUS;
    else f->denom->sign = f->numer->sign = PLUS;
}

fraction* floatstr_to_fraction(char *str) {
    fraction *f = new_fraction();
    f->numer = new_bigint();
    f->denom = str_to_bigint("1");
    bool underpoint = 0;
    char *it = str;
    if (*it == '-') f->numer->sign = 1, it++;
    for (; *it!='\0'; it++) {
        if (*it=='.') {underpoint = 1; continue;}
        push_back(f->numer->head, *it-'0');
        if (underpoint) push_back(f->denom->head, 0);
    }
    reduce(f);
    return f;
}

fraction* FADD(fraction *a, fraction *b) {
    fraction *r = new_fraction();

    bigint *tmp1 = naiveMUL(a->numer, b->denom);
    bigint *tmp2 = naiveMUL(b->numer, a->denom);
    r->numer = ADD(tmp1, tmp2);
    r->denom = naiveMUL(a->denom, b->denom);
    free_bigint(tmp1), free_bigint(tmp2);

    reduce(r);
    return r;
}
fraction* FSUB(fraction *a, fraction *b) {
    fraction *r = new_fraction();

    bigint *tmp1 = naiveMUL(a->numer, b->denom);
    bigint *tmp2 = naiveMUL(b->numer, a->denom);
    r->numer = SUB(tmp1, tmp2);
    r->denom = naiveMUL(a->denom, b->denom);
    free_bigint(tmp1), free_bigint(tmp2);

    reduce(r);
    return r;
}
fraction* FMUL(fraction *a, fraction *b) {
    fraction *r = new_fraction();

    r->numer = naiveMUL(a->numer, b->numer);
    r->denom = naiveMUL(a->denom, b->denom);
    
    reduce(r);
    return r;
}
fraction* FDIV(fraction *a, fraction *b) {
    fraction *r = new_fraction();

    r->numer = naiveMUL(a->numer, b->denom);
    r->denom = naiveMUL(a->denom, b->numer);
    
    reduce(r);
    return r;
}

void print_fraction(fraction *f, long long dec) {
    reduce(f);
    print_bigint(f->numer);
    printf("/");
    print_bigint(f->denom);
}

/*
// TESTCODE
int main() {
    fraction *a = bigint_to_fraction(str_to_bigint("10"));
    fraction *b = bigint_to_fraction(str_to_bigint("35"));
    fraction *c = FDIV(a, b);
    print_fraction(c, 0);
}
// */