#include "bigint.c"

void propagate_carry(bigint *n) {
    // 매번 받아올림을 제때에 수행해주면 곱셈 과정에서 각 노드 data의 최댓값 < 128
    // 다행히도 char 자료형 범위를 벗어나지 않을 걸로 예상하는데..?
    node *it = back(n->head);
    int carry = 0;

    while (it != n->head) {
        it->data += carry;
        carry = it->data / 10;
        it->data %= 10;

        it = it->prev;
    }

    while (carry > 0) {
        push_front(n->head, carry % 10);
        carry /= 10;
    }
}

void propagate_carry_from_iter(bigint *n, node *it) {
    // 받아온 it에서부터 시작하기.
    int carry = 0;

    while (it != n->head) {
        it->data += carry;
        carry = it->data / 10;
        it->data %= 10;

        it = it->prev;
    }

    while (carry > 0) {
        push_front(n->head, carry % 10);
        carry /= 10;
    }
}

// O(NM)
bigint* naiveMUL(bigint *a, bigint *b) {
    shrink_to_fit(a), shrink_to_fit(b);

    bigint *r = new_bigint();
    push_back(r->head, 0);
    r->sign = a->sign ^ b->sign;

    node *a_it, *b_it, *r_it, *r_st;

    b_it = back(b->head);
    r_st = back(r->head);
    while (b_it != b->head) {
        r_it = r_st;
        for (a_it=back(a->head); a_it!=a->head; a_it=a_it->prev) {
            r_it->data += a_it->data * b_it->data;
            if (r_it->prev == r->head) push_front(r->head, 0);
            r_it = r_it->prev;
        }
        propagate_carry_from_iter(r, r_st);
        b_it = b_it->prev;
        if (r_st->prev == r->head) push_front(r->head, 0);
        r_st = r_st->prev;
    }
    shrink_to_fit(r);
    return r;
}


void karatsuba() {
    
}

/*
// TESTCODE
int main() {
    bigint *a = str_to_bigint("2434234");
    bigint *b = str_to_bigint("88");
    bigint *r = naiveMUL(a, b);
    // 88 * 2434234 = 214212592
    // 999 * 999 = 998001
    print_bigint(r);
}
*/