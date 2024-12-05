#include "divmod.c"

// 유리수 사칙연산만으로 나올 수 있는 수는 유리수로 한정된다.
// 분수 자료형을 만들어서 관리하면 이론 상 정밀도가 손실되지 않고 수를 관리할 수 있다.
typedef struct fraction {
    // numer : 분자, denom : 분모
    bigint *numer, *denom;
} fraction;

fraction* new_fraction() {
    fraction *ret = (fraction*)malloc(sizeof(fraction));
    if (ret == NULL) AllocateFailed();
    return ret;
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
    if (iszero(f->denom)) ZeroDivision();
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

    // 출력 자릿수를 음수로 지정 시 분수 형태로 출력하게.
    if (dec < 0) {
        print_bigint(f->numer);
        printf("/");
        print_bigint(f->denom);
        return;
    }

    // 출력 자릿수를 지정한 경우.
    // 지정 자릿수 이하는 그냥 잘라낸다. (반올림 X)
    bigint *x = ABS(f->numer);
    bigint *y = ABS(f->denom);
    bigint *mod = new_bigint();

    // list도 되지만, leading zero 제거의 편의를 위하여..
    bigint *int_part = new_bigint();
    bigint *dec_part = new_bigint();

    node *it = front(x->head);
    while (it != x->head || dec>0) {
        // 종이에서 계산하는 나눗셈 과정.
        // mod는 계산 과정 중 쓰는 수이면서
        // 모든 계산이 끝나면 나머지가 될 것임.
        if (it!=x->head) push_back(mod->head, it->data);
        else {
            dec--;
            push_back(mod->head, 0);
        }
        shrink_to_fit(mod);
        int curq = 0;
        while (!LESS(mod, y)) {
            bigint *tmp = SUB(mod, y);
            free_bigint(mod);
            mod = tmp;
            curq++;
        }

        if (it != x->head) {
            it = it->next;
            push_back(int_part->head, curq);
        }
        else {
            push_front(dec_part->head, curq);
        }
    }
    free_bigint(x), free_bigint(y);

    shrink_to_fit(int_part);
    // 나누어떨어진 경우는 소수점 아래 붙은 불필요한 0을 삭제.
    if (iszero(mod)) {
        shrink_to_fit(dec_part);
        if (iszero(dec_part)) {
            // 소수점 이하가 다 0이라면
            if (!iszero(int_part) && f->numer->sign == MINUS) printf("-");
            for (node *it=front(int_part->head); it!=int_part->head; it=it->next) printf("%d", it->data);
        }
        else {
            if (f->numer->sign == MINUS) printf("-");
            for (node *it=front(int_part->head); it!=int_part->head; it=it->next) printf("%d", it->data);
            printf(".");
            for (node *it=back(dec_part->head); it!=dec_part->head; it=it->prev) printf("%d", it->data);
        }
    }
    // 그렇지 않은 경우 무조건 최대 길이까지 출력.
    else {
        if (f->numer->sign == MINUS) printf("-");
        for (node *it=front(int_part->head); it!=int_part->head; it=it->next) printf("%d", it->data);
        printf(".");
        for (node *it=back(dec_part->head); it!=dec_part->head; it=it->prev) printf("%d", it->data);
    }
    free_bigint(mod);
    free_bigint(int_part), free_bigint(dec_part);
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