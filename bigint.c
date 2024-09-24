#include "linked_list.c"

typedef struct bigint {
    bool sign;
    node *head;
} bigint;

// create new empty bigint and init with new EMPTY list, not 0
bigint* new_bigint() {
    bigint* n = (bigint*)malloc(sizeof(bigint));
    n->sign = 0;
    n->head = new_list();
    return n;
}
void release_bigint(bigint *n) {
    destroy_list(n->head);
    free(n);
}
void reset_bigint(bigint *n) {
    n->sign = 0;
    clear(n->head);
}

// remove leading zeros and make -0 to +0 
void shrink_to_fit(bigint *n) {
    node *head = n->head;
    while (!empty(head) && back(head)->data==0) pop_back(head);
    if (empty(head)) n->sign = 0, push_back(head, 0);
}

// !warning! must store it in variable before using it. don't pass directly as fuction arguments.
bigint* str_to_bigint(char *str) {
    bigint *n = new_bigint();
    char *it = str;
    if (*it == '-') n->sign = 1, it++;
    for (; *it!='\0'; it++) push_front(n->head, *it-'0');
    shrink_to_fit(n);
    return n;
}

void print_bigint(bigint *n) {
    shrink_to_fit(n);
    if (n->sign) putchar('-');
    for (node *it=n->head->prev; it!=n->head; it=it->prev) putchar(it->data+'0');
}

// !warning! must store it in variable before using it. don't pass directly as fuction arguments.
bigint* NEG(bigint *n) {
    shrink_to_fit(n);
    bigint *ret = new_bigint();
    destroy_list(ret->head);
    ret->sign = n->sign ^ 1;
    ret->head = copy(n->head);
    shrink_to_fit(ret);
    return ret;
}
void NEGATE(bigint *n) {
    n->sign ^= 1;
    shrink_to_fit(n);
}
// !warning! must store it in variable before using it. don't pass directly as fuction arguments.
bigint* ABS(bigint *n) {
    shrink_to_fit(n);
    bigint *ret = new_bigint();
    destroy_list(ret->head);
    ret->head = copy(n->head);
    return ret;
}

// a < b
bool LESS(bigint *a, bigint *b) {
    shrink_to_fit(a), shrink_to_fit(b);
    if (a->sign == 1 && b->sign == 0) return 1;
    if (a->sign == 0 && b->sign == 1) return 0;

    bool absgtr = 0, equ = 0;
    node *p1 = a->head->next, *p2 = b->head->next;
    while (p1!=a->head || p2!=b->head) {
        if (p1==a->head) {absgtr = 1; break;}
        if (p2==b->head) {absgtr = 0; break;}
        p1 = p1->next, p2 = p2->next;
    }

    if (p1==a->head && p2==b->head) {
        p1 = a->head->prev, p2 = b->head->prev;
        while (p1!=a->head && p2!=b->head) {
            if (p1->data == p2->data) {
                p1 = p1->prev, p2 = p2->prev;
                continue;
            }
            absgtr = p1->data < p2->data;
            break;
        }
        if (p1==a->head && p2==b->head) equ = 1;
    }
    if (equ) return 0;
    else return absgtr^a->sign;
}

// !warning! must store it in variable before using it. don't pass directly as fuction arguments.
bigint* SUB(bigint *a, bigint *b);

// !warning! must store it in variable before using it. don't pass directly as fuction arguments.
bigint* ADD(bigint *a, bigint *b) {
    shrink_to_fit(a), shrink_to_fit(b);
    if (a->sign != b->sign) {
        if (a->sign == 1) {
            NEGATE(a);
            bigint *ret = SUB(b, a); 
            NEGATE(a);
            return ret;
        }
        else {
            NEGATE(b);
            bigint *ret = SUB(a, b);
            NEGATE(b);
            return ret;
        } 
    }

    bigint *r = new_bigint();
    r->sign = a->sign;

    node *p1 = a->head->next, *p2 = b->head->next;
    bool carry = 0;
    char v1 = 0, v2 = 0, cur;
    while (1) {
        v1 = p1!=a->head ? p1->data : 0;
        v2 = p2!=b->head ? p2->data : 0;

        cur = v1+v2+carry;
        carry = cur>=10;
        push_back(r->head, cur % 10);

        if (!carry && p1==a->head && p2==b->head) break;
        if (p1 != a->head) p1 = p1->next;
        if (p2 != b->head) p2 = p2->next;
    }

    shrink_to_fit(r);
    return r;
}

// !warning! must store it in variable before using it. don't pass directly as fuction arguments.
bigint* SUB(bigint *a, bigint *b) {
    // automatically changes to 0 <= B <= A, A-B
    if (a->sign == 1 && b->sign == 1) {
        NEGATE(a), NEGATE(b);
        bigint *ret = SUB(b, a);
        NEGATE(a), NEGATE(b);
        return ret;
    }
    if (a->sign != b->sign) {
        NEGATE(b);
        bigint *ret = ADD(a, b);
        NEGATE(b);
        return ret;
    }
    if (LESS(a,b)) {
        bigint *ret = SUB(b,a);
        NEGATE(ret);
        return ret;
    }

    bigint *r = new_bigint();

    node *p1 = a->head->next, *p2 = b->head->next;
    bool carry = 0;
    char v1 = 0, v2 = 0, cur;
    while (1) {
        v1 = p1!=a->head ? p1->data : 0;
        v2 = p2!=b->head ? p2->data : 0;

        cur = v1-v2 - carry;
        carry = cur<0;
        push_back(r->head, (cur + 10)%10);

        if (!carry && p1==a->head && p2==b->head) break;
        if (p1 != a->head) p1 = p1->next;
        if (p2 != b->head) p2 = p2->next;
    }

    shrink_to_fit(r);
    return r;
}

// 메모리 관리 실수를 줄이기 위한 아이디어
/*
void store_ADD_to_target(bigint *a, bigint *b, bigint *target)
와 같이 선언하여
결과값을 리턴하는 게 아니라
res에 결과값을 담아서 쓰도록 강제하면?
*/


// TESTCODE
/*
int main() {
    bigint *a = str_to_bigint("-31");
    bigint *b = str_to_bigint("44");
    bigint *r = ADD(a, b);
    print_bigint(r);

    release_bigint(r);
}
*/