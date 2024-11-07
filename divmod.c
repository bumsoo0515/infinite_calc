#include "multiply.c"

// O(NM)
void naiveDIVMOD(bigint *a, bigint *b, bigint **div_p, bigint **mod_p) {
    bigint *x = ABS(a);
    bigint *y = ABS(b);

    bigint *div = new_bigint();
    bigint *mod = new_bigint();

    node *it = front(x->head);
    while (it != x->head) {
        // 종이에서 계산하는 나눗셈 과정.
        // mod는 계산 과정 중 쓰는 수이면서
        // 모든 계산이 끝나면 나머지가 될 것임.
        push_back(mod->head, it->data);
        shrink_to_fit(mod);
        int curq = 0;
        while (!LESS(mod, y)) {
            bigint *tmp = SUB(mod, y);
            free_bigint(mod);
            mod = tmp;
            curq++;
        }

        push_back(div->head, curq);
        it = it->next;
    }

    free_bigint(x), free_bigint(y);

    bool sgn = a->sign ^ b->sign;
    div->sign = mod->sign = sgn;
    shrink_to_fit(div), shrink_to_fit(mod);
    if (*div_p != NULL) free_bigint(*div_p);
    if (*mod_p != NULL) free_bigint(*mod_p);
    *div_p = div;
    *mod_p = mod;
}

bigint* GCD(bigint *a, bigint *b) {
    bigint *x = ABS(a);
    bigint *y = ABS(b);

    while (!iszero(y)) {
        bigint *d = NULL, *m = NULL;
        naiveDIVMOD(x, y, &d, &m);
        free_bigint(d); d = NULL;
        free_bigint(x); x = NULL;
        x = y;
        y = m;
    }

    free_bigint(y);
    return x;
}

/*
// TESTCODE
int main() {
    bigint *a = str_to_bigint("10");
    bigint *b = str_to_bigint("5");

    bigint *div = NULL, *mod = str_to_bigint("123123");
    naiveDIVMOD(a, b, &div, &mod);

    print_bigint(div);
    putchar('\n');
    print_bigint(mod);

    free_bigint(a), free_bigint(b);
    free_bigint(div), free_bigint(mod);
}
// */