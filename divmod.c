#include "multiply.c"

// O(NM), 아직 양수 간의 연산만 구현
void naiveDIVMOD(bigint *a, bigint *b, bigint **div_p, bigint **mod_p) {
    shrink_to_fit(a), shrink_to_fit(b);

    bigint *div = new_bigint();
    bigint *mod = new_bigint();

    node *it = a->head->prev;
    while (it != a->head) {
        push_front(mod->head, it->data);
        shrink_to_fit(mod);

        int curq = 0;
        while (!LESS(mod, b)) {
            bigint *tmp = SUB(mod, b);
            release_bigint(mod);
            mod = tmp;
            curq++;
        }

        push_front(div->head, curq);
        it = it->prev;
    }

    shrink_to_fit(div), shrink_to_fit(mod);
    if (*div_p != NULL) release_bigint(*div_p);
    if (*mod_p != NULL) release_bigint(*mod_p);
    *div_p = div;
    *mod_p = mod;
}

int main() {
    bigint *a = str_to_bigint("103420");
    bigint *b = str_to_bigint("66");

    bigint *div = NULL, *mod = str_to_bigint("123123");
    naiveDIVMOD(a, b, &div, &mod);

    print_bigint(div);
    putchar('\n');
    print_bigint(mod);
}